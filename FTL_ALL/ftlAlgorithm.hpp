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
        SAFE_DELETE_ARRAY(m_pBmpData);
        SAFE_DELETE_ARRAY(m_pResultPalette);
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
                //ZeroMemory(m_pBmpData, nBmpDataSize);
                CopyMemory(m_pBmpData, pBmpData, nBmpDataSize);
            }
            else{
                SetLastError(ERROR_NOT_ENOUGH_MEMORY);
                return FALSE;
            }
        }

        return TRUE;
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
        table = NULL;
        pixels = NULL;
        quantizedPixels = NULL;
        cubes = NULL;
        imageWidth = 0;
        imageSize = 0;
        pixelIndex = 0;
    }

    CFWuColorQuantizer::~CFWuColorQuantizer()
    {

    }

    BOOL CFWuColorQuantizer::ProcessQuantizer(UINT nWantClrCount, UINT *pResultClrCount)
    {
        FUNCTION_BLOCK_TRACE(100);
        BOOL bRet = FALSE;
        API_VERIFY(OnPrepare());
        if (bRet)
        {
            API_VERIFY(OnProcessQuantizer(nWantClrCount, pResultClrCount));

            OnFinish();
        }
        return bRet;
    }

    BOOL CFWuColorQuantizer::OnProcessQuantizer(UINT nWantClrCount, UINT *pResultClrCount)
    {
        FUNCTION_BLOCK_TRACE(100);
        BOOL bRet = FALSE;
        CalculateMoments();

        int next = 0;
        std::vector<float> volumeVariance(MAX_COLOR);

        // processes the cubes
        for (int cubeIndex = 1; cubeIndex < nWantClrCount; ++cubeIndex)
        {
            // if cut is possible; make it
            if (Cut(cubes[next], cubes[cubeIndex]))
            {
                volumeVariance[next] = cubes[next].Volume > 1 ? CalculateVariance(cubes[next]) : 0.0f;
                volumeVariance[cubeIndex] = cubes[cubeIndex].Volume > 1 ? CalculateVariance(cubes[cubeIndex]) : 0.0f;
            }
            else // the cut was not possible, revert the index
            {
                volumeVariance[next] = 0.0f;
                cubeIndex--;
            }

            next = 0;
            float temp = volumeVariance[0];

            for (int index = 1; index <= cubeIndex; ++index)
            {
                if (volumeVariance[index] > temp)
                {
                    temp = volumeVariance[index];
                    next = index;
                }
            }

            if (temp <= 0.0)
            {
                nWantClrCount = cubeIndex + 1;
                break;
            }
        }

        std::vector<int> lookupRed(MAX_COLOR);
        std::vector<int> lookupGreen(MAX_COLOR);
        std::vector<int> lookupBlue(MAX_COLOR);
        tag = new int[MAX_VOLUME];
        ZeroMemory(tag, sizeof(int) * MAX_VOLUME);
        //std::vector<int> tag(MAX_VOLUME);
        
        // precalculates lookup tables
        for (int k = 0; k < nWantClrCount; ++k)
        {
            Mark(cubes[k], k, tag);

            long weight = Volume(cubes[k], weights);

            if (weight > 0)
            {
                lookupRed[k] = (int)(Volume(cubes[k], momentsRed) / weight);
                lookupGreen[k] = (int)(Volume(cubes[k], momentsGreen) / weight);
                lookupBlue[k] = (int)(Volume(cubes[k], momentsBlue) / weight);
            }
            else
            {
                lookupRed[k] = 0;
                lookupGreen[k] = 0;
                lookupBlue[k] = 0;
            }
        }

        // copies the per pixel tags 
        for (int index = 0; index < imageSize; ++index)
        {
            quantizedPixels[index] = tag[quantizedPixels[index]];
        }

        reds.resize(nWantClrCount + 1);
        greens.resize(nWantClrCount + 1);
        blues.resize(nWantClrCount + 1);
        sums.resize(nWantClrCount + 1);
        indices.resize(imageSize);

        // scans and adds colors
        for (int index = 0; index < imageSize; index++)
        {
            COLORREF color = pixels[index];
            //Color color = Color.FromArgb(pixels[index]);

            int match = quantizedPixels[index];
            int bestMatch = match;
            int bestDistance = 100000000;

            for (int lookup = 0; lookup < nWantClrCount; lookup++)
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

            reds[bestMatch] += GetRValue(color);
            greens[bestMatch] += GetGValue(color);
            blues[bestMatch] += GetBValue(color);
            sums[bestMatch]++;

            indices[index] = bestMatch;
        }
        
        m_pResultPalette = new COLORREF[nWantClrCount];
        ZeroMemory(m_pResultPalette, sizeof(COLORREF) * nWantClrCount);
        m_nPaletteItemCount = 0;

        //List<Color> result = new List<Color>();

        // generates palette
        for (int paletteIndex = 0; paletteIndex < nWantClrCount; paletteIndex++)
        {
            if (sums[paletteIndex] > 0)
            {
                reds[paletteIndex] /= sums[paletteIndex];
                greens[paletteIndex] /= sums[paletteIndex];
                blues[paletteIndex] /= sums[paletteIndex];
            }

            COLORREF color = MAKE_RGBA(reds[paletteIndex], greens[paletteIndex], blues[paletteIndex], 0xFF);
            //Color color = Color.FromArgb(255, );
            m_pResultPalette[m_nPaletteItemCount++] = color;
            //result.Add(color);
        }

        pixelIndex = 0;

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
        cubes = new ColorCube[MAX_COLOR];

        // initializes all the cubes
        //for (int cubeIndex = 0; cubeIndex < MAX_COLOR; cubeIndex++)
        //{
        //    cubes[cubeIndex] = new WuColorCube();
        //}

        // resets the reference minimums
        cubes[0].RedMinimum = 0;
        cubes[0].GreenMinimum = 0;
        cubes[0].BlueMinimum = 0;

        // resets the reference maximums
        cubes[0].RedMaximum = MAX_SIDE_INDEX;
        cubes[0].GreenMaximum = MAX_SIDE_INDEX;
        cubes[0].BlueMaximum = MAX_SIDE_INDEX;

        _InitMomentSize(weights);
        _InitMomentSize(momentsRed);
        _InitMomentSize(momentsGreen);
        _InitMomentSize(momentsBlue);
        _InitMomentSize(moments);

        table = new int[256];

        for (int tableIndex = 0; tableIndex < 256; ++tableIndex)
        {
            table[tableIndex] = tableIndex * tableIndex;
        }

        pixelIndex = 0;
        imageWidth = m_nWidth;
        imageSize = m_nWidth * m_nHeight;

        quantizedPixels = new int[imageSize];
        pixels = new UINT[imageSize];


        //OnAddColor
        
        //int nRowBytes = (nWidth * bpp + 31) >> 5 << 2;  //4字节对齐，计算每行的字节数 //<< 2
        int nPixOffset = (m_nBpp / 8);
        for (int h = 0; h < m_nHeight; h++)
        {
            for (int w = 0; w < m_nWidth; w++)
            {
                byte* pBuf = (m_pBmpData + h * m_nBmpLineBytes);
                //TODO:
                int Red = *(pBuf + 2);
                int Green = *(pBuf+1);
                int Blue = *(pBuf);

                int indexRed = (Red >> 3) + 1;
                int indexGreen = (Green >> 3) + 1;
                int indexBlue = (Blue >> 3) + 1;

                weights[indexRed][indexGreen][indexBlue]++;
                momentsRed[indexRed][indexGreen][indexBlue] += Red;
                momentsGreen[indexRed][indexGreen][indexBlue] += Green;
                momentsBlue[indexRed][indexGreen][indexBlue] += Blue;
                moments[indexRed][indexGreen][indexBlue] += table[Red] + table[Green] + table[Blue];

                quantizedPixels[pixelIndex] = (indexRed << 10) + (indexRed << 6) + indexRed + (indexGreen << 5) + indexGreen + indexBlue;
                pixels[pixelIndex] = MAKE_RGBA(Red, Green, Blue, 0xFF);
                pixelIndex++;
            }
        }

        return TRUE;
    }

    void CFWuColorQuantizer::CalculateMoments()
    {
        FUNCTION_BLOCK_TRACE(100);
        std::vector<long> area(SIDE_SIZE);
        std::vector<long> areaRed(SIDE_SIZE);
        std::vector<long> areaGreen(SIDE_SIZE);
        std::vector<long> areaBlue(SIDE_SIZE);
        std::vector<float> area2(SIDE_SIZE);

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
                long line = 0;
                long lineRed = 0;
                long lineGreen = 0;
                long lineBlue = 0;
                float line2 = 0.0f;

                for (int blueIndex = 1; blueIndex <= MAX_SIDE_INDEX; ++blueIndex)
                {
                    line += weights[redIndex][greenIndex][blueIndex];
                    lineRed += momentsRed[redIndex][greenIndex][blueIndex];
                    lineGreen += momentsGreen[redIndex][greenIndex][blueIndex];
                    lineBlue += momentsBlue[redIndex][greenIndex][blueIndex];
                    line2 += moments[redIndex][greenIndex][blueIndex];

                    area[blueIndex] += line;
                    areaRed[blueIndex] += lineRed;
                    areaGreen[blueIndex] += lineGreen;
                    areaBlue[blueIndex] += lineBlue;
                    area2[blueIndex] += line2;

                    weights[redIndex][greenIndex][blueIndex] = weights[redIndex - 1][greenIndex][blueIndex] + area[blueIndex];
                    momentsRed[redIndex][greenIndex][blueIndex] = momentsRed[redIndex - 1][greenIndex][blueIndex] + areaRed[blueIndex];
                    momentsGreen[redIndex][greenIndex][blueIndex] = momentsGreen[redIndex - 1][greenIndex][blueIndex] + areaGreen[blueIndex];
                    momentsBlue[redIndex][greenIndex][ blueIndex] = momentsBlue[redIndex - 1][greenIndex][blueIndex] + areaBlue[blueIndex];
                    moments[redIndex][greenIndex][blueIndex] = moments[redIndex - 1][greenIndex][blueIndex] + area2[blueIndex];
                }
            }
        }
    }
    void CFWuColorQuantizer::OnFinish()
    {

    }

    long CFWuColorQuantizer::Volume(ColorCube& cube, ColorMomentType& moment)
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

    float CFWuColorQuantizer::VolumeFloat(ColorCube& cube, VolumeMomentType& moment)
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

    long CFWuColorQuantizer::Top(ColorCube& cube, int direction,  int position, ColorMomentType& moment)
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
    long CFWuColorQuantizer::Bottom(ColorCube &cube, int direction, ColorMomentType& moment)
    {
        switch (direction)
        {
        case DIR_RED:
            return (
                -moment[cube.RedMinimum][cube.GreenMaximum][cube.BlueMaximum] 
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

    float CFWuColorQuantizer::CalculateVariance(ColorCube& cube)
    {
        float volumeRed = Volume(cube, momentsRed);
        float volumeGreen = Volume(cube, momentsGreen);
        float volumeBlue = Volume(cube, momentsBlue);
        float volumeMoment = VolumeFloat(cube, moments);
        float volumeWeight = Volume(cube, weights);

        float distance = volumeRed*volumeRed + volumeGreen*volumeGreen + volumeBlue*volumeBlue;

        return volumeMoment - (distance/volumeWeight);
    }

    float CFWuColorQuantizer::Maximize(ColorCube& cube, int direction, int first, int last, 
        int* cut, long wholeRed, long wholeGreen, long wholeBlue, long wholeWeight)
    {
        long bottomRed = Bottom(cube, direction, momentsRed);
        long bottomGreen = Bottom(cube, direction, momentsGreen);
        long bottomBlue = Bottom(cube, direction, momentsBlue);
        long bottomWeight = Bottom(cube, direction, weights);

        float result = 0.0f;
        *cut = -1;

        for (int position = first; position < last; ++position)
        {
            // determines the cube cut at a certain position
            long halfRed = bottomRed + Top(cube, direction, position, momentsRed);
            long halfGreen = bottomGreen + Top(cube, direction, position, momentsGreen);
            long halfBlue = bottomBlue + Top(cube, direction, position, momentsBlue);
            long halfWeight = bottomWeight + Top(cube, direction, position, weights);

            // the cube cannot be cut at bottom (this would lead to empty cube)
            if (halfWeight != 0)
            {
                float halfDistance = halfRed*halfRed + halfGreen*halfGreen + halfBlue*halfBlue;
                float temp = halfDistance/halfWeight;

                halfRed = wholeRed - halfRed;
                halfGreen = wholeGreen - halfGreen;
                halfBlue = wholeBlue - halfBlue;
                halfWeight = wholeWeight - halfWeight;

                if (halfWeight != 0)
                {
                    halfDistance = halfRed * halfRed + halfGreen * halfGreen + halfBlue * halfBlue;
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

        long wholeRed = Volume(first, momentsRed);
        long wholeGreen = Volume(first, momentsGreen);
        long wholeBlue = Volume(first, momentsBlue);
        long wholeWeight = Volume(first, weights);

        float maxRed = Maximize(first, DIR_RED, first.RedMinimum + 1, first.RedMaximum, &cutRed, wholeRed, wholeGreen, wholeBlue, wholeWeight);
        float maxGreen = Maximize(first, DIR_GREEN, first.GreenMinimum + 1, first.GreenMaximum, &cutGreen, wholeRed, wholeGreen, wholeBlue, wholeWeight);
        float maxBlue = Maximize(first, DIR_BLUE, first.BlueMinimum + 1, first.BlueMaximum, &cutBlue, wholeRed, wholeGreen, wholeBlue, wholeWeight);

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

    void CFWuColorQuantizer::Mark(ColorCube& cube, int label, int* tag)
    {
        for (int redIndex = cube.RedMinimum + 1; redIndex <= cube.RedMaximum; ++redIndex)
        {
            for (int greenIndex = cube.GreenMinimum + 1; greenIndex <= cube.GreenMaximum; ++greenIndex)
            {
                for (int blueIndex = cube.BlueMinimum + 1; blueIndex <= cube.BlueMaximum; ++blueIndex)
                {
                    tag[(redIndex << 10) + (redIndex << 6) + redIndex + (greenIndex << 5) + greenIndex + blueIndex] = label;
                }
            }
        }
    }

}



#endif //FTL_ALGORITHM_HPP