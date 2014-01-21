package com.fishjam.storehelper;

import android.R.integer;
import android.app.IntentService;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.drawable.Drawable;
import android.widget.ImageView;

class PositionInfo
{
	public int nFloor;
	public Point ptPosition = new Point();
}

public class PositionView extends ImageView {
	//private int mCurPositionX = 0;
	//private int mCurPositionY = 0;
	PositionInfo mPositionInfo = new PositionInfo();
	Paint mPaint = new Paint();
	
	int [] FloorNumber = {
			R.raw.floor_1,
			R.raw.floor_2
	};
	
	public PositionView(Context context) {
		super(context);
		mPositionInfo.nFloor = -1;
		setAdjustViewBounds(true);
		
		//setBackgroundDrawable(drawFlow);
	}
	
	public void SetPosition(PositionInfo posInfo)
	{
		//if(mPositionInfo.nFloor != posInfo.nFloor){
			Resources resources = getResources();
			Drawable drawFloor =  resources.getDrawable(FloorNumber[posInfo.nFloor]);
			setImageDrawable(drawFloor);
		//}
		mPositionInfo = posInfo;
		
		postInvalidate();
	}

	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		if (mPositionInfo.ptPosition.x != 0 && mPositionInfo.ptPosition.y != 0) {
			//paint.setColor(getResources().getColor(R.drawable.red));
			mPaint.setColor(getResources().getColor(R.drawable.red));
			canvas.drawCircle(mPositionInfo.ptPosition.x, mPositionInfo.ptPosition.y, 10, mPaint);
		}
	}
	
}
