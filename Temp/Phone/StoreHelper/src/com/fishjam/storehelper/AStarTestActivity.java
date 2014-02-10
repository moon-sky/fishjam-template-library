package com.fishjam.storehelper;

import javax.security.auth.PrivateCredentialPermission;

import com.fishjam.util.AStar;

import android.R.integer;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Rect;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.View;

class AStarIconItem extends StartIconInfo {
	private final static String TAG = AStarIconItem.class.getSimpleName();
	
	AStarIconItem(Activity activity) {
		super(activity);
		
		this.icon = R.drawable.gas_station;
		this.name = mActivity.getResources().getString(R.string.AStar);
	}

	@Override
	void onExecute() {
		
		super.onExecute();
		Intent intent = new Intent(mActivity, AStarTestActivity.class);
		mActivity.startActivity(intent);
	}
}

class AStarView extends View{
	private final static String TAG = AStarView.class.getSimpleName();
	
	private static final int OFFSET_X = 20;
	private static final int OFFSET_Y = 20;
	private static final int GRID_WIDTH = 10;
	private static final int GRID_HEIGHT = 10;
	
	private Paint mPaintEmpty;
	private Paint mPaintWall;
	private Paint mPaintRoute;
	private Canvas mCanvasMemory;
	private Bitmap mBitmapMemory;  
	
	AStar.PosInfo mAStarPosInfo[];
	private AStar mAStar;
	private int mWidth = 15;
	private int mHeight = 25;
	 
	private int[][] mMapAcotor = {
			{ 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0 },
			{ 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0 },
			{ 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0 },
			{ 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 0 },
			{ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
			{ 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1 },
			{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
			{ 1, 8, 8, 1, 9, 0, 1, 1, 1, 1, 9, 8, 8, 8, 8 },
			{ 1, 8, 8, 8, 1, 0, 0, 0, 0, 0, 1, 8, 8, 8, 8 },
			{ 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 5, 6, 7 },
			{ 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
			{ 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 8, 8, 8, 8, 9, 0, 0, 0, 0, 0, 9, 8, 8, 8, 8 },
			{ 8, 8, 8, 8, 1, 0, 0, 0, 0, 0, 1, 8, 8, 8, 8 },
			{ 5, 6, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4 },
			{ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 },
			{ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } 
		};
	
	AStarView(Context context){
		super(context);
		setKeepScreenOn(true);
		
		mPaintEmpty = new Paint();
		mPaintEmpty.setColor(Color.WHITE);
		mPaintWall = new Paint();
		mPaintWall.setColor(Color.BLACK);
		mPaintRoute = new Paint();
		mPaintRoute.setColor(Color.RED);
		Display display =  ((Activity)context).getWindowManager().getDefaultDisplay();
		mBitmapMemory = Bitmap.createBitmap(display.getWidth(), display.getHeight(), Bitmap.Config.RGB_565);
    	
		//mBitmapMemory = Bitmap.createBitmap(GRID_WIDTH * mWidth, GRID_HEIGHT * mHeight, Bitmap.Config.RGB_565);
		mCanvasMemory = new Canvas(mBitmapMemory);
		mAStar = new AStar(mMapAcotor, 1, 1, 23, 13, true);
		//mAStar.searchPath();
		mAStarPosInfo = mAStar.getResult(true);
		if (mAStarPosInfo != null) {
			Log.i(TAG, "mAStarPosInfo is not null, step " + mAStarPosInfo.length);
		}
		else {
			Log.e(TAG, "mAStarPosInfo is NULL");
		}
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		//Rect rectGrid = new Rect();
		int nStartLine = 0;
		int nStartCol = 0;
		Paint paint;
		
		for (int i = 0; i < mHeight; i++) {
			mCanvasMemory.drawText("" + i  , (mWidth + 1) * GRID_WIDTH,  (i + 1) * GRID_HEIGHT , mPaintRoute);
		}
		for (int j = 0; j < mWidth; j++) {
			mCanvasMemory.drawText("" + j  ,  j*GRID_WIDTH, GRID_HEIGHT *  (mHeight + 1), mPaintRoute);
		}
		//mCanvasMemory.drawText("fishjmam test", GRID_WIDTH * 18, GRID_HEIGHT * 18, mPaintRoute);
		for (int i = 0; i < mHeight; i++) {
			for (int j = 0; j < mWidth; j++) {
				nStartLine = j * GRID_WIDTH;
				nStartCol = i * GRID_HEIGHT;
				if(mMapAcotor[i][j] == 0)
				{
					paint = mPaintEmpty;
				}else
				{
					paint = mPaintWall;
				}
				mCanvasMemory.drawRect(nStartLine, nStartCol, (j +1) * GRID_WIDTH, (i + 1) * GRID_HEIGHT, paint);
			}
		}
		
		if (mAStarPosInfo != null) {
			StringBuilder stringBuilder = new StringBuilder();
			
			for (int i = mAStarPosInfo.length - 1; i >=0 ; i--) {
				nStartLine = mAStarPosInfo[i].col * GRID_WIDTH;
				nStartCol = mAStarPosInfo[i].row * GRID_HEIGHT;
				stringBuilder.append("=>[" + mAStarPosInfo[i].row + "," + mAStarPosInfo[i].col + "," + 
						mAStar.getDirString(mAStarPosInfo[i].dir)  +  "]");
				mCanvasMemory.drawRect(nStartLine + GRID_HEIGHT / 4 , nStartCol + GRID_WIDTH / 4 ,
						nStartLine + GRID_WIDTH  * 3 / 4,  nStartCol + GRID_HEIGHT * 3 /4, 
						mPaintRoute );
			}
			Log.i(TAG, stringBuilder.toString());
		}
		canvas.drawBitmap(mBitmapMemory, 0, 0, null);
	}
}

public class AStarTestActivity extends Activity {
	private final static String TAG = AStarTestActivity.class.getSimpleName();
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {

		super.onCreate(savedInstanceState);
		setContentView(new AStarView(this));
	}
}

