package com.fishjam.storehelper;

import com.fishjam.util.AStar;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.drawable.Drawable;
import android.widget.ImageView;

public class PositionView extends ImageView {
	Paint mPaint = new Paint();
	int    mCurFloor = -1;
	AStar mAStar;
	
	//StoreInformation	mStoreInformation;
	
	int [] FloorNumber = {
			R.raw.floor_1,
			R.raw.floor_2
	};
	
	public PositionView(Context context) {
		super(context);
		setAdjustViewBounds(true);
	}

	public int[][] mMap = {
			{ 5, 6, 7, 0, 3, 4, 3, 4, 3, 4, 0, 0, 2, 2, 0 },
			{ 13, 14, 15, 0, 11, 12, 11, 12, 11, 12, 0, 0, 10, 10, 0 },
			{ 21, 22, 23, 0, 19, 20, 19, 20, 19, 20, 0, 0, 18, 18, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
			{ 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10 },
			{ 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18 },
			{ 8, 8, 8, 8, 9, 0, 0, 0, 0, 0, 9, 8, 8, 8, 8 },
			{ 8, 8, 8, 8, 17, 0, 0, 0, 0, 0, 17, 8, 8, 8, 8 },
			{ 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 5, 6, 7 },
			{ 11, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 13, 14, 15 },
			{ 19, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 21, 22, 23 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 8, 8, 8, 8, 9, 0, 0, 0, 0, 0, 9, 8, 8, 8, 8 },
			{ 8, 8, 8, 8, 17, 0, 0, 0, 0, 0, 17, 8, 8, 8, 8 },
			{ 5, 6, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4 },
			{ 13, 14, 15, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 12 },
			{ 21, 22, 23, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 20 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } 
	};

	
	
	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		StoreInformation storeInformation = StoreInformation.Instance();
		Point ptCurPos = storeInformation.mCurPositionInfo.ptPos;
		Point ptCarPos = storeInformation.mCarPositionInfo.ptPos;

		Resources resources = getResources();
		if (ptCurPos.x >= 0 && ptCurPos.y >= 0) {
			mPaint.setColor(resources.getColor(R.color.red));
			canvas.drawCircle(ptCurPos.x, ptCurPos.y, 10, mPaint);
		}
		if (ptCarPos.x >= 0 && ptCarPos.y >= 0) {
			mPaint.setColor(resources.getColor(R.color.grgray));
			canvas.drawCircle(ptCarPos.x, ptCarPos.y, 10, mPaint);
		}
	}

	public void setFloor(int iFloor) {
		if (mCurFloor != iFloor) {
			mCurFloor = iFloor;
			
			Resources resources = getResources();
			Drawable drawFloor =  resources.getDrawable(FloorNumber[mCurFloor]);
			setImageDrawable(drawFloor);
			postInvalidate();
		}
		
	}
	
}
