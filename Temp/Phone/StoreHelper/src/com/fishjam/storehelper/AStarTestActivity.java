package com.fishjam.storehelper;

import android.R.integer;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.View;

import com.fishjam.util.AStar;

class AStarIconItem extends StartIconInfo {
	//private final static String TAG = AStarIconItem.class.getSimpleName();
	
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
	
	//private static final int OFFSET_X = 20;
	//private static final int OFFSET_Y = 20;
	
	private Paint mPaintEmpty;
	private Paint mPaintWall;
	private Paint mPaintRoute;
	private Canvas mCanvasMemory;
	private Bitmap mBitmapMemory;  
	
	AStar.PosInfo mAStarPosInfo[];
	int[][] mTileData;
	private AStar mAStar;
	private int mWidth;
	private int mHeight;
	
	
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
		mTileData = FloorMapManager.getInstance().getMapByFloor(0).mTileData;
		mWidth = mTileData[0].length;
		mHeight = mTileData.length;
		
		mAStar = new AStar(mTileData, 16, 15, 47, 44, true);
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
			mCanvasMemory.drawText("" + (i%10)  , (mWidth + 1) * GlobalConfig.TILE_WIDTH,  (i + 1) * GlobalConfig.TILE_HEIGHT , mPaintRoute);
		}
		for (int j = 0; j < mWidth; j++) {
			mCanvasMemory.drawText("" + (j%10)  ,  j*GlobalConfig.TILE_WIDTH, GlobalConfig.TILE_HEIGHT *  (mHeight + 1), mPaintRoute);
		}
		//mCanvasMemory.drawText("fishjmam test", GRID_WIDTH * 18, GRID_HEIGHT * 18, mPaintRoute);
		for (int i = 0; i < mHeight; i++) {
			for (int j = 0; j < mWidth; j++) {
				nStartLine = j * GlobalConfig.TILE_WIDTH;
				nStartCol = i * GlobalConfig.TILE_HEIGHT;
				if(mTileData[i][j] == 0)
				{
					paint = mPaintEmpty;
				}else
				{
					paint = mPaintWall;
				}
				mCanvasMemory.drawRect(nStartLine, nStartCol, (j +1) * GlobalConfig.TILE_WIDTH, (i + 1) * GlobalConfig.TILE_HEIGHT, paint);
			}
		}
		
		if (mAStarPosInfo != null) {
			StringBuilder stringBuilder = new StringBuilder();
			
			for (int i = mAStarPosInfo.length - 1; i >=0 ; i--) {
				nStartLine = mAStarPosInfo[i].col * GlobalConfig.TILE_WIDTH;
				nStartCol = mAStarPosInfo[i].row * GlobalConfig.TILE_HEIGHT;
				stringBuilder.append("=>[" + mAStarPosInfo[i].row + "," + mAStarPosInfo[i].col + "," + 
						mAStar.getDirString(mAStarPosInfo[i].dir)  +  "]");
				mCanvasMemory.drawRect(nStartLine + GlobalConfig.TILE_HEIGHT / 4 , nStartCol + GlobalConfig.TILE_WIDTH / 4 ,
						nStartLine + GlobalConfig.TILE_WIDTH  * 3 / 4,  nStartCol + GlobalConfig.TILE_HEIGHT * 3 /4, 
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
		
		Log.i(TAG, "AStarTestActivity.onCreate");
	}
}

