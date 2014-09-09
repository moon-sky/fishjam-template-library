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
    }

    CFColorQuantizerBase::~CFColorQuantizerBase(){
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
                ZeroMemory(m_pBmpData, nBmpDataSize);
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

    void CFWuColorQuantizer::Mark(ColorCube& cube, int label, std::vector<int>& tag)
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