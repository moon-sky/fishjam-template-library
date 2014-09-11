#ifndef FTL_ALGORITHM_HPP
#define FTL_ALGORITHM_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlAlgorithm.h"
#endif


namespace FTL
{
    IFColorQuantizer::~IFColorQuantizer(){

    }

    CFColorQuantizerBase::CFColorQuantizerBase(){
        m_nWidth = 0;
        m_nHeight = 0;
        m_nBpp = 0;
        m_nBmpDataSize = 0;
        m_nBmpLineBytes = 0;
        m_nPaletteItemCount = 0;

        m_pBmpData = NULL;
        m_pResultPalette = NULL;
        m_pResultBuffer = NULL;
    }

    CFColorQuantizerBase::~CFColorQuantizerBase(){
        SAFE_DELETE_ARRAY(m_pResultBuffer);
        SAFE_DELETE_ARRAY(m_pResultPalette);
        SAFE_DELETE_ARRAY(m_pBmpData);
    }

    BOOL CFColorQuantizerBase::SetBmpInfo(UINT nWidth, UINT nHeight, UINT nBpp, BYTE* pBmpData, UINT nBmpDataSize)
    {
        BOOL bRet = FALSE;

        UINT nBmpLineBytes = CALC_BMP_ALLIGNMENT_WIDTH_COUNT(nWidth, nBpp);
        UINT nBmpDataSizeCheck = nBmpLineBytes * nHeight; 
        FTLASSERT(nBmpDataSizeCheck == nBmpDataSize && TEXT("BMP图片的大小需要四字节对齐"));

        if (nBmpDataSizeCheck != nBmpDataSize)
        {
            SetLastError(ERROR_INVALID_PARAMETER);
            return FALSE;
        }

        SAFE_DELETE_ARRAY(m_pBmpData);

        if (nBmpDataSize > 0)
        {
            m_pBmpData = new BYTE[nBmpDataSize];
            if (m_pBmpData)
            {
                m_nWidth = nWidth;
                m_nHeight = nHeight;
                m_nBpp = nBpp;
                m_nBmpLineBytes = nBmpLineBytes;
                m_nBmpDataSize = nBmpDataSize;
                CopyMemory(m_pBmpData, pBmpData, nBmpDataSize);
            }
            else{
                SetLastError(ERROR_NOT_ENOUGH_MEMORY);
                return FALSE;
            }
        }

        return TRUE;
    }

    BOOL CFColorQuantizerBase::ProcessQuantizer(UINT colorCount, UINT *pResultClrCount)
    {
        FUNCTION_BLOCK_TRACE(100);
        BOOL bRet = FALSE;
        API_VERIFY(OnPrepare());
        if (bRet)
        {
            API_VERIFY(OnProcessQuantizer(colorCount, pResultClrCount));

            OnFinish();
        }
        return bRet;
    }

    BOOL CFColorQuantizerBase::OnPrepare()
    {
        return TRUE;
    }
    void CFColorQuantizerBase::OnFinish()
    {
    }

    COLORREF* CFColorQuantizerBase::GetPalette(UINT* pResultCount)
    {
        if (pResultCount && m_pResultPalette)
        {
            *pResultCount = m_nPaletteItemCount;
        }
        return m_pResultPalette;
    }

    //------------------------------------  CFWuColorQuantizer ---------------------------------------------------------------- //

    CFWuColorQuantizer::CFWuColorQuantizer()
    {
        m_table = NULL;
        m_pixels = NULL;
        m_quantizedPixels = NULL;
        m_cubes = NULL;
        m_tag = NULL;
        m_imageWidth = 0;
        m_imageSize = 0;
        m_pixelIndex = 0;
    }

    CFWuColorQuantizer::~CFWuColorQuantizer()
    {
        SAFE_DELETE_ARRAY(m_tag);
        SAFE_DELETE_ARRAY(m_cubes);
        SAFE_DELETE_ARRAY(m_quantizedPixels);
        SAFE_DELETE_ARRAY(m_pixels);
        SAFE_DELETE_ARRAY(m_table);
    }


    BOOL CFWuColorQuantizer::OnProcessQuantizer(UINT colorCount, UINT *pResultClrCount)
    {
        FUNCTION_BLOCK_TRACE(100);
        BOOL bRet = FALSE;
        CalculateMoments();

        int next = 0;
        std::vector<double> volumeVariance(MAX_COLOR);

        int nCalcCount = 0;
        // processes the cubes
        for (UINT cubeIndex = 1; cubeIndex < colorCount; ++cubeIndex)
        {
            nCalcCount++;
            // if cut is possible; make it
            if (Cut(m_cubes[next], m_cubes[cubeIndex]))
            {
                volumeVariance[next] = m_cubes[next].Volume > 1 ? CalculateVariance(m_cubes[next]) : 0.0f;
                volumeVariance[cubeIndex] = m_cubes[cubeIndex].Volume > 1 ? CalculateVariance(m_cubes[cubeIndex]) : 0.0f;
            }
            else // the cut was not possible, revert the index
            {
                volumeVariance[next] = 0.0f;
                cubeIndex--;
            }

            next = 0;
            double temp = volumeVariance[0];

            for (UINT index = 1; index <= cubeIndex; ++index)
            {
                if (volumeVariance[index] > temp)
                {
                    temp = volumeVariance[index];
                    next = index;
                }
            }

            if (temp <= 0.0)
            {
                colorCount = cubeIndex + 1;
                break;
            }
        }

        std::vector<int> lookupRed(MAX_COLOR);
        std::vector<int> lookupGreen(MAX_COLOR);
        std::vector<int> lookupBlue(MAX_COLOR);
        m_tag = new int[MAX_VOLUME];
        ZeroMemory(m_tag, sizeof(int) * MAX_VOLUME);
        //std::vector<int> tag(MAX_VOLUME);
        
        // precalculates lookup tables
        for (UINT k = 0; k < colorCount; ++k)
        {
            Mark(m_cubes[k], k, m_tag);

            INT64 weight = Volume(m_cubes[k], m_weights);

            if (weight > 0)
            {
                lookupRed[k] = (int)(Volume(m_cubes[k], m_momentsRed) / weight);
                lookupGreen[k] = (int)(Volume(m_cubes[k], m_momentsGreen) / weight);
                lookupBlue[k] = (int)(Volume(m_cubes[k], m_momentsBlue) / weight);
            }
            else
            {
                lookupRed[k] = 0;
                lookupGreen[k] = 0;
                lookupBlue[k] = 0;
            }
        }

        // copies the per pixel tags 
        for (int index = 0; index < m_imageSize; ++index)
        {
            int tagIndex = m_quantizedPixels[index];
            int newIndex = m_tag[tagIndex];
            m_quantizedPixels[index] = newIndex;
        }

        m_reds.resize(colorCount + 1);
        m_greens.resize(colorCount + 1);
        m_blues.resize(colorCount + 1);
        m_sums.resize(colorCount + 1);
        m_indices.resize(m_imageSize);

        // scans and adds colors
        for (int index = 0; index < m_imageSize; index++)
        {
            COLORREF color = m_pixels[index];
            //Color color = Color.FromArgb(pixels[index]);

            int match = m_quantizedPixels[index];
            int bestMatch = match;
            int bestDistance = 100000000;

            for (UINT lookup = 0; lookup < colorCount; lookup++)
            {
                int foundRed = lookupRed[lookup];
                int foundGreen = lookupGreen[lookup];
                int foundBlue = lookupBlue[lookup];
                int deltaRed = GetRValue(color) - foundRed;
                int deltaGreen = GetGValue(color) - foundGreen;
                int deltaBlue = GetBValue(color) - foundBlue;

                int distance = deltaRed * deltaRed + deltaGreen * deltaGreen + deltaBlue * deltaBlue;

                if (distance < bestDistance)
                {
                    bestDistance = distance;
                    bestMatch = lookup;
                }
            }

            m_reds[bestMatch] += GetRValue(color);
            m_greens[bestMatch] += GetGValue(color);
            m_blues[bestMatch] += GetBValue(color);
            m_sums[bestMatch]++;

            m_indices[index] = bestMatch;
        }
        
        m_pResultPalette = new COLORREF[colorCount];
        ZeroMemory(m_pResultPalette, sizeof(COLORREF) * colorCount);
        m_nPaletteItemCount = 0;

        //List<Color> result = new List<Color>();

        // generates palette
        for (UINT paletteIndex = 0; paletteIndex < colorCount; paletteIndex++)
        {
            if (m_sums[paletteIndex] > 0)
            {
                m_reds[paletteIndex] /= m_sums[paletteIndex];
                m_greens[paletteIndex] /= m_sums[paletteIndex];
                m_blues[paletteIndex] /= m_sums[paletteIndex];
            }

            COLORREF color = MAKE_RGBA(m_reds[paletteIndex], m_greens[paletteIndex], m_blues[paletteIndex], 0xFF);
            int R = GetRValue(color);
            int G = GetGValue(color);
            int B = GetBValue(color);
            //Color color = Color.FromArgb(255, );
            m_pResultPalette[m_nPaletteItemCount++] = color;
            //result.Add(color);
        }

        m_pixelIndex = 0;

        return TRUE;
    }

    void CFWuColorQuantizer::_InitMomentSize(ColorMomentType& moments)
    {
        moments.resize(SIDE_SIZE);
        for (int i = 0; i < SIDE_SIZE; i++)
        {
            moments[i].resize(SIDE_SIZE);
            for (int j = 0; j < SIDE_SIZE; j++)
            {
                moments[i][j].resize(SIDE_SIZE);
            }
        }
    }
    void CFWuColorQuantizer::_InitMomentSize(VolumeMomentType& moments)
    {
        moments.resize(SIDE_SIZE);
        for (int i = 0; i < SIDE_SIZE; i++)
        {
            moments[i].resize(SIDE_SIZE);
            for (int j = 0; j < SIDE_SIZE; j++)
            {
                moments[i][j].resize(SIDE_SIZE);
            }
        }
    }
    BOOL CFWuColorQuantizer::OnPrepare()
    {
        FUNCTION_BLOCK_TRACE(100);
        // creates all the cubes
        m_cubes = new ColorCube[MAX_COLOR];
        
        // initializes all the cubes
        //for (int cubeIndex = 0; cubeIndex < MAX_COLOR; cubeIndex++)
        //{
        //    cubes[cubeIndex] = new WuColorCube();
        //}

        // resets the reference minimums
        m_cubes[0].RedMinimum = 0;
        m_cubes[0].GreenMinimum = 0;
        m_cubes[0].BlueMinimum = 0;

        // resets the reference maximums
        m_cubes[0].RedMaximum = MAX_SIDE_INDEX;
        m_cubes[0].GreenMaximum = MAX_SIDE_INDEX;
        m_cubes[0].BlueMaximum = MAX_SIDE_INDEX;

        _InitMomentSize(m_weights);
        _InitMomentSize(m_momentsRed);
        _InitMomentSize(m_momentsGreen);
        _InitMomentSize(m_momentsBlue);
        _InitMomentSize(m_moments);

        m_table = new int[256];
        ZeroMemory(m_table, sizeof(int) * 256);
        for (int tableIndex = 0; tableIndex < 256; ++tableIndex)
        {
            m_table[tableIndex] = tableIndex * tableIndex;
        }

        m_pixelIndex = 0;
        m_imageWidth = m_nWidth;
        m_imageSize = m_nWidth * m_nHeight;

        m_quantizedPixels = new int[m_imageSize];
        ZeroMemory(m_quantizedPixels, sizeof(int) * m_imageSize);
        m_pixels = new COLORREF[m_imageSize];
        ZeroMemory(m_pixels, sizeof(COLORREF) * m_imageSize);


        //OnAddColor
        
        //int nRowBytes = (nWidth * bpp + 31) >> 5 << 2;  //4字节对齐，计算每行的字节数 //<< 2
        int nPixOffset = (m_nBpp / 8);
        BYTE* pBuf = m_pBmpData;
        for (UINT h = 0; h < m_nHeight; h++)
        {
            for (UINT w = 0; w < m_nWidth; w++)
            {
                //byte* pBuf = (m_pBmpData + h * m_nBmpLineBytes + w * nPixOffset);
                //TODO:
                int Red = *(pBuf + 2);
                int Green = *(pBuf+1);
                int Blue = *(pBuf);

                int indexRed = (Red >> 3) + 1;
                int indexGreen = (Green >> 3) + 1;
                int indexBlue = (Blue >> 3) + 1;

                m_weights[indexRed][indexGreen][indexBlue]++;
                m_momentsRed[indexRed][indexGreen][indexBlue] += Red;
                m_momentsGreen[indexRed][indexGreen][indexBlue] += Green;
                m_momentsBlue[indexRed][indexGreen][indexBlue] += Blue;
                m_moments[indexRed][indexGreen][indexBlue] += m_table[Red] + m_table[Green] + m_table[Blue];

                m_quantizedPixels[m_pixelIndex] = (indexRed << 10) + (indexRed << 6) + indexRed + (indexGreen << 5) + indexGreen + indexBlue;
                m_pixels[m_pixelIndex] = MAKE_RGBA(Red, Green, Blue, 0xFF);
                int R = GetRValue(m_pixels[m_pixelIndex]);
                int G = GetGValue(m_pixels[m_pixelIndex]);
                int B = GetBValue(m_pixels[m_pixelIndex]);

                m_pixelIndex++;
                pBuf += nPixOffset;
            }
        }

        return TRUE;
    }

    void CFWuColorQuantizer::CalculateMoments()
    {
        FUNCTION_BLOCK_TRACE(100);
        std::vector<INT64> area(SIDE_SIZE);
        std::vector<INT64> areaRed(SIDE_SIZE);
        std::vector<INT64> areaGreen(SIDE_SIZE);
        std::vector<INT64> areaBlue(SIDE_SIZE);
        std::vector<double> area2(SIDE_SIZE);

        for (int redIndex = 1; redIndex <= MAX_SIDE_INDEX; ++redIndex)
        {
            for (int index = 0; index <= MAX_SIDE_INDEX; ++index)
            {
                area[index] = 0;
                areaRed[index] = 0;
                areaGreen[index] = 0;
                areaBlue[index] = 0;
                area2[index] = 0;
            }

            for (int greenIndex = 1; greenIndex <= MAX_SIDE_INDEX; ++greenIndex)
            {
                INT64 line = 0;
                INT64 lineRed = 0;
                INT64 lineGreen = 0;
                INT64 lineBlue = 0;
                double line2 = 0.0f;

                for (int blueIndex = 1; blueIndex <= MAX_SIDE_INDEX; ++blueIndex)
                {
                    line += m_weights[redIndex][greenIndex][blueIndex];
                    lineRed += m_momentsRed[redIndex][greenIndex][blueIndex];
                    lineGreen += m_momentsGreen[redIndex][greenIndex][blueIndex];
                    lineBlue += m_momentsBlue[redIndex][greenIndex][blueIndex];
                    line2 += m_moments[redIndex][greenIndex][blueIndex];

                    area[blueIndex] += line;
                    areaRed[blueIndex] += lineRed;
                    areaGreen[blueIndex] += lineGreen;
                    areaBlue[blueIndex] += lineBlue;
                    area2[blueIndex] += line2;

                    m_weights[redIndex][greenIndex][blueIndex] = m_weights[redIndex - 1][greenIndex][blueIndex] + area[blueIndex];
                    m_momentsRed[redIndex][greenIndex][blueIndex] = m_momentsRed[redIndex - 1][greenIndex][blueIndex] + areaRed[blueIndex];
                    m_momentsGreen[redIndex][greenIndex][blueIndex] = m_momentsGreen[redIndex - 1][greenIndex][blueIndex] + areaGreen[blueIndex];
                    m_momentsBlue[redIndex][greenIndex][ blueIndex] = m_momentsBlue[redIndex - 1][greenIndex][blueIndex] + areaBlue[blueIndex];
                    m_moments[redIndex][greenIndex][blueIndex] = m_moments[redIndex - 1][greenIndex][blueIndex] + area2[blueIndex];
                }
            }
        }
    }
    void CFWuColorQuantizer::OnFinish()
    {

    }

    INT64 CFWuColorQuantizer::Volume(ColorCube& cube, ColorMomentType& moment)
    {
        return moment[cube.RedMaximum][cube.GreenMaximum][cube.BlueMaximum]
            - moment[cube.RedMaximum][cube.GreenMaximum][cube.BlueMinimum]
            - moment[cube.RedMaximum][cube.GreenMinimum][cube.BlueMaximum]
            + moment[cube.RedMaximum][cube.GreenMinimum][cube.BlueMinimum]
            - moment[cube.RedMinimum][cube.GreenMaximum][cube.BlueMaximum]
            + moment[cube.RedMinimum][cube.GreenMaximum][cube.BlueMinimum]
            + moment[cube.RedMinimum][cube.GreenMinimum][cube.BlueMaximum]
            - moment[cube.RedMinimum][cube.GreenMinimum][cube.BlueMinimum];
    }

    double CFWuColorQuantizer::VolumeFloat(ColorCube& cube, VolumeMomentType& moment)
    {
        return moment[cube.RedMaximum][cube.GreenMaximum][cube.BlueMaximum]
            - moment[cube.RedMaximum][cube.GreenMaximum][cube.BlueMinimum]
            - moment[cube.RedMaximum][cube.GreenMinimum][cube.BlueMaximum]
            + moment[cube.RedMaximum][cube.GreenMinimum][cube.BlueMinimum]
            - moment[cube.RedMinimum][cube.GreenMaximum][cube.BlueMaximum]
            + moment[cube.RedMinimum][cube.GreenMaximum][cube.BlueMinimum]
            + moment[cube.RedMinimum][cube.GreenMinimum][cube.BlueMaximum]
            - moment[cube.RedMinimum][cube.GreenMinimum][cube.BlueMinimum];
    }

    INT64 CFWuColorQuantizer::Top(ColorCube& cube, int direction,  int position, ColorMomentType& moment)
    {
        switch (direction)
        {
        case DIR_RED:
            return (moment[position][cube.GreenMaximum][cube.BlueMaximum]
            - moment[position][cube.GreenMaximum][cube.BlueMinimum]
            - moment[position][cube.GreenMinimum][cube.BlueMaximum]
            + moment[position][cube.GreenMinimum][cube.BlueMinimum]);

        case DIR_GREEN:
            return (moment[cube.RedMaximum][position][cube.BlueMaximum]
            - moment[cube.RedMaximum][position][cube.BlueMinimum]
            - moment[cube.RedMinimum][position][cube.BlueMaximum]
            + moment[cube.RedMinimum][position][cube.BlueMinimum]);

        case DIR_BLUE:
            return (moment[cube.RedMaximum][cube.GreenMaximum][position]
            - moment[cube.RedMaximum][cube.GreenMinimum][position]
            - moment[cube.RedMinimum][cube.GreenMaximum][ position]
            + moment[cube.RedMinimum][cube.GreenMinimum][position]);

        default:
            return 0;
        }
    }
    INT64 CFWuColorQuantizer::Bottom(ColorCube &cube, int direction, ColorMomentType& moment)
    {
        switch (direction)
        {
        case DIR_RED:
            return (-moment[cube.RedMinimum][cube.GreenMaximum][cube.BlueMaximum] 
            + moment[cube.RedMinimum][cube.GreenMaximum][cube.BlueMinimum]
            + moment[cube.RedMinimum][cube.GreenMinimum][cube.BlueMaximum]
            - moment[cube.RedMinimum][cube.GreenMinimum][cube.BlueMinimum]);

        case DIR_GREEN:
            return (-moment[cube.RedMaximum][cube.GreenMinimum][cube.BlueMaximum]
            + moment[cube.RedMaximum][cube.GreenMinimum][cube.BlueMinimum]
            + moment[cube.RedMinimum][cube.GreenMinimum][cube.BlueMaximum]
            - moment[cube.RedMinimum][cube.GreenMinimum][cube.BlueMinimum]);

        case DIR_BLUE:
            return (-moment[cube.RedMaximum][cube.GreenMaximum][cube.BlueMinimum]
            + moment[cube.RedMaximum][cube.GreenMinimum][cube.BlueMinimum]
            + moment[cube.RedMinimum][cube.GreenMaximum][cube.BlueMinimum]
            - moment[cube.RedMinimum][cube.GreenMinimum][cube.BlueMinimum]);

        default:
            return 0;
        }
    }

    double CFWuColorQuantizer::CalculateVariance(ColorCube& cube)
    {
        double volumeRed = (double)Volume(cube, m_momentsRed);
        double volumeGreen = (double)Volume(cube, m_momentsGreen);
        double volumeBlue = (double)Volume(cube, m_momentsBlue);
        double volumeMoment = VolumeFloat(cube, m_moments);
        double volumeWeight = (double)Volume(cube, m_weights);

        double distance = volumeRed*volumeRed + volumeGreen*volumeGreen + volumeBlue*volumeBlue;

        return volumeMoment - (distance/volumeWeight);
    }

    double CFWuColorQuantizer::Maximize(ColorCube& cube, int direction, int first, int last, 
        int* cut, INT64 wholeRed, INT64 wholeGreen, INT64 wholeBlue, INT64 wholeWeight)
    {
        INT64 bottomRed = Bottom(cube, direction, m_momentsRed);
        INT64 bottomGreen = Bottom(cube, direction, m_momentsGreen);
        INT64 bottomBlue = Bottom(cube, direction, m_momentsBlue);
        INT64 bottomWeight = Bottom(cube, direction, m_weights);

        double result = 0.0f;
        *cut = -1;

        for (int position = first; position < last; ++position)
        {
            // determines the cube cut at a certain position
            INT64 halfRed = bottomRed + Top(cube, direction, position, m_momentsRed);
            INT64 halfGreen = bottomGreen + Top(cube, direction, position, m_momentsGreen);
            INT64 halfBlue = bottomBlue + Top(cube, direction, position, m_momentsBlue);
            INT64 halfWeight = bottomWeight + Top(cube, direction, position, m_weights);

            // the cube cannot be cut at bottom (this would lead to empty cube)
            if (halfWeight != 0)
            {
                double halfDistance = (double)halfRed*halfRed + halfGreen*halfGreen + halfBlue*halfBlue;
                double temp = halfDistance/halfWeight;

                halfRed = wholeRed - halfRed;
                halfGreen = wholeGreen - halfGreen;
                halfBlue = wholeBlue - halfBlue;
                halfWeight = wholeWeight - halfWeight;

                if (halfWeight != 0)
                {
                    halfDistance = (double)halfRed * halfRed + halfGreen * halfGreen + halfBlue * halfBlue;
                    temp += halfDistance / halfWeight;

                    if (temp > result)
                    {
                        result = temp;
                        *cut = position;
                    }
                }
            }
        }

        return result;
    }

    BOOL CFWuColorQuantizer::Cut(ColorCube& first, ColorCube& second)
    {
        int direction;

        int cutRed = 0;
        int cutGreen = 0;
        int cutBlue = 0;

        INT64 wholeRed = Volume(first, m_momentsRed);
        INT64 wholeGreen = Volume(first, m_momentsGreen);
        INT64 wholeBlue = Volume(first, m_momentsBlue);
        INT64 wholeWeight = Volume(first, m_weights);

        double maxRed = Maximize(first, DIR_RED, first.RedMinimum + 1, first.RedMaximum, &cutRed, wholeRed, wholeGreen, wholeBlue, wholeWeight);
        double maxGreen = Maximize(first, DIR_GREEN, first.GreenMinimum + 1, first.GreenMaximum, &cutGreen, wholeRed, wholeGreen, wholeBlue, wholeWeight);
        double maxBlue = Maximize(first, DIR_BLUE, first.BlueMinimum + 1, first.BlueMaximum, &cutBlue, wholeRed, wholeGreen, wholeBlue, wholeWeight);

        if ((maxRed >= maxGreen) && (maxRed >= maxBlue))
        {
            direction = DIR_RED;

            // cannot split empty cube
            if (cutRed < 0){
                return FALSE; 
            }
        }
        else
        {
            if ((maxGreen >= maxRed) && (maxGreen >= maxBlue))
            {
                direction = DIR_GREEN;
            }
            else
            {
                direction = DIR_BLUE;
            }
        }

        second.RedMaximum = first.RedMaximum;
        second.GreenMaximum = first.GreenMaximum;
        second.BlueMaximum = first.BlueMaximum;

        // cuts in a certain direction
        switch (direction)
        {
        case DIR_RED:
            second.RedMinimum = first.RedMaximum = cutRed;
            second.GreenMinimum = first.GreenMinimum;
            second.BlueMinimum = first.BlueMinimum;
            break;

        case DIR_GREEN:
            second.GreenMinimum = first.GreenMaximum = cutGreen;
            second.RedMinimum = first.RedMinimum;
            second.BlueMinimum = first.BlueMinimum;
            break;

        case DIR_BLUE:
            second.BlueMinimum = first.BlueMaximum = cutBlue;
            second.RedMinimum = first.RedMinimum;
            second.GreenMinimum = first.GreenMinimum;
            break;
        }

        // determines the volumes after cut
        first.Volume = (first.RedMaximum - first.RedMinimum)*(first.GreenMaximum - first.GreenMinimum)*(first.BlueMaximum - first.BlueMinimum);
        second.Volume = (second.RedMaximum - second.RedMinimum)*(second.GreenMaximum - second.GreenMinimum)*(second.BlueMaximum - second.BlueMinimum);

        // the cut was successfull
        return TRUE;
    }

    void CFWuColorQuantizer::Mark(ColorCube& cube, int label, int* pTag)
    {
        for (int redIndex = cube.RedMinimum + 1; redIndex <= cube.RedMaximum; ++redIndex)
        {
            for (int greenIndex = cube.GreenMinimum + 1; greenIndex <= cube.GreenMaximum; ++greenIndex)
            {
                for (int blueIndex = cube.BlueMinimum + 1; blueIndex <= cube.BlueMaximum; ++blueIndex)
                {
                    int nIndex = (redIndex << 10) + (redIndex << 6) + redIndex + (greenIndex << 5) + greenIndex + blueIndex;
                    pTag[nIndex] = label;
                }
            }
        }
    }

}



#endif //FTL_ALGORITHM_HPP