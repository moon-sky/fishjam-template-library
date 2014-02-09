package com.fishjam.storehelper;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.ImageView.ScaleType;
import com.fishjam.util.*;


public class PositionActivity extends Activity{
    DragImageSurfaceView mImageSurfaceView;
    
    static final String KEY_CAR_POSITION = "carPosition";
    static final String KEY_CUR_POSITION = "curPosition";
    static final String KEY_FLOOR = "Floor";

    //static final String KEY_CODE = "Code";
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        /*
        mImageSurfaceView = new DragImageSurfaceView(this);
        setContentView(mImageSurfaceView);
        
        //*/
        PositionView positionView = new PositionView(this);
        positionView.setScaleType(ScaleType.CENTER);
        setContentView(positionView);
        Intent intent = getIntent();
        int [] curPosition = intent.getExtras().getIntArray(KEY_CUR_POSITION);
        int [] carPosition = intent.getExtras().getIntArray(KEY_CAR_POSITION);
        int nFloor = intent.getExtras().getInt(KEY_FLOOR);
        PositionInfo posInfo = new PositionInfo();
        posInfo.nFloor = nFloor;
        
        if (curPosition != null) {
            posInfo.ptCurPosition.set(curPosition[0], curPosition[1]);
        }
        if (carPosition != null) {
			posInfo.ptCarPosition.set(carPosition[0], carPosition[1]);
		}
        positionView.SetPosition(posInfo);
    }

}
