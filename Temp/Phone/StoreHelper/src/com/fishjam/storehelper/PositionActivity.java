package com.fishjam.storehelper;

import com.fishjam.util.GestureImageView;

import android.app.Activity;
import android.os.Bundle;
import android.widget.ImageView;

public class PositionActivity extends Activity{

	@Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        PositionView positionView = new PositionView(this);
        //positionView.setScaleType(ImageView.ScaleType.CENTER);
        positionView.setFloor(StoreInformation.Instance(this).mCurPositionInfo.iFloor);
        setContentView(positionView);
        
    }

}
