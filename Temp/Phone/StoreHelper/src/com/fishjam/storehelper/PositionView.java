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
	public Point ptCarPosition = new Point();
	public Point ptCurPosition = new Point();
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
		if (mPositionInfo.ptCurPosition.x != 0 && mPositionInfo.ptCurPosition.y != 0) {
			//paint.setColor(getResources().getColor(R.drawable.red));
			Resources resources = getResources();
			mPaint.setColor(resources.getColor(R.color.red));
			canvas.drawCircle(mPositionInfo.ptCurPosition.x, mPositionInfo.ptCurPosition.y, 10, mPaint);
			
			mPaint.setColor(resources.getColor(R.color.grgray));
			canvas.drawCircle(mPositionInfo.ptCarPosition.x, mPositionInfo.ptCarPosition.y, 10, mPaint);
		}
	}
	
}
