package com.fishjam.storehelper;

import android.R.integer;
import android.app.IntentService;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.drawable.Drawable;
import android.widget.ImageView;



public class PositionView extends ImageView {
	private int mCurPositionX = 0;
	private int mCurPositionY = 0;
	
	public PositionView(Context context) {
		super(context);
		setAdjustViewBounds(true);
		Resources resources = getResources();
		Drawable drawFloor =  resources.getDrawable(R.raw.floor_1);
		setImageDrawable(drawFloor);
		//setBackgroundDrawable(drawFlow);
	}
	
	public void SetPosition(int x, int y)
	{
		mCurPositionX = x;
		mCurPositionY = y;
		postInvalidate();
	}

	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		if (mCurPositionX != 0 && mCurPositionY != 0) {
			Paint paint = new Paint();
			paint.setColor(getResources().getColor(android.R.color.holo_blue_bright));
			canvas.drawCircle(mCurPositionX,  mCurPositionY, 10, paint);
		}
	}
	
}
