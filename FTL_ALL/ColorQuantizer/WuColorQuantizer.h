#pragma once

#include "ColorQuantizerBase.h"

/*******************************************************************************************************
* Xiaolin Wu -- http://www.ece.mcmaster.ca/~xwu/cq.c
*******************************************************************************************************/
namespace FTL
{
    class CFWuColorQuantizer : public CFColorQuantizerBase{
    public:
        CFWuColorQuantizer();
        virtual ~CFWuColorQuantizer();
    protected:
        virtual BOOL OnPrepare();
        virtual BOOL OnProcessQuantizer(UINT colorCount, UINT *pResultClrCount);
        virtual void OnFinish();
    private:
        enum {
            MAX_COLOR = 512,
            DIR_RED = 2,
            DIR_GREEN = 1,
            DIR_BLUE = 0,
            SIDE_SIZE = 33,
            MAX_SIDE_INDEX = 32,
            MAX_VOLUME = SIDE_SIZE * SIDE_SIZE * SIDE_SIZE,
        };

        //template<typename T>
        //using MomentType = std::vector<std::vector<std::vector<T> > >; //long moment[SIDE_SIZE][SIDE_SIZE][SIDE_SIZE]);
        typedef std::vector<std::vector<std::vector<INT64> > > ColorMomentType;
        typedef std::vector<std::vector<std::vector<double> > > VolumeMomentType;

        struct ColorCube{
            ColorCube(){
                RedMinimum = 0;
                RedMaximum = 0;
                GreenMinimum = 0;
                GreenMaximum = 0;
                BlueMinimum = 0;
                BlueMaximum = 0;
                Volume = 0;
            }
            int RedMinimum;
            int RedMaximum;
            int GreenMinimum;
            int GreenMaximum;
            int BlueMinimum;
            int BlueMaximum;
            int Volume;
        };

        INT64 Bottom(ColorCube &cube, int direction, ColorMomentType& moment);
        INT64 Top(ColorCube& cube, int direction,  int position, ColorMomentType& moment);

        INT64 Volume(ColorCube& cube, ColorMomentType& moment);
        double VolumeFloat(ColorCube& cube, VolumeMomentType& moment);
        double CalculateVariance(ColorCube& cube);
        double Maximize(ColorCube& cube, int direction, int first, int last, 
            int* cut, INT64 wholeRed, INT64 wholeGreen, INT64 wholeBlue, INT64 wholeWeight);
        BOOL Cut(ColorCube& first, ColorCube& second);
        void Mark(ColorCube& cube, int label, int* pTag);

        void CalculateMoments();
        void _InitMomentSize(ColorMomentType& moments);
        void _InitMomentSize(VolumeMomentType& moments);
        //private:
    public:
        std::vector<int> m_reds;
        std::vector<int> m_greens;
        std::vector<int> m_blues;
        std::vector<int> m_sums;
        std::vector<int> m_indices;

        ColorMomentType m_weights;
        ColorMomentType m_momentsRed;
        ColorMomentType m_momentsGreen;
        ColorMomentType m_momentsBlue;
        VolumeMomentType m_moments;

        int* m_tag;
        int* m_quantizedPixels;
        int*             m_table;
        COLORREF* m_pixels;

        int m_imageWidth;
        int m_imageSize;
        int m_pixelIndex;

        ColorCube*       m_cubes;
    };
}
