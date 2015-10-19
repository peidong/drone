package com.example.bt;

import java.io.IOException;
import java.util.Set;
import java.util.UUID;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.view.View.OnClickListener;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;
import android.mtp.MtpDevice;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {

	BluetoothAdapter mAdapter;
	Button mButton;
	private TextView text;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		
		
		mAdapter= BluetoothAdapter.getDefaultAdapter();
		
		
		if (!mAdapter.isEnabled()) {  
            // 我们通过startActivityForResult()方法发起的Intent将会在onActivityResult()回调方法中获取用户的选择，比如用户单击了Yes开启，  
            // 那么将会收到RESULT_OK的结果，  
            // 如果RESULT_CANCELED则代表用户不愿意开启蓝牙  
            Intent mIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);  
            startActivityForResult(mIntent, 1);
            
            // 用enable()方法来开启，无需询问用户(实惠无声息的开启蓝牙设备),这时就需要用到android.permission.BLUETOOTH_ADMIN权限。  
			//mAdapter.enable();  
            // mBluetoothAdapter.disable();//关闭蓝牙  
        }  
		mButton = (Button) this.findViewById(R.id.a);  
        mButton.setOnClickListener(listenera);
        mButton = (Button) this.findViewById(R.id.b);  
        mButton.setOnClickListener(listenerb);
        
        
        
        
	}
	View.OnClickListener listenera = new OnClickListener() {  
		  
        @Override  
        public void onClick(View v) {  
            // TODO Auto-generated method stub  
        	//启动修改蓝牙可见性的Intent
    		Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
    		//设置蓝牙可见性的时间，方法本身规定最多可见300秒
    		intent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 10);
    		startActivity(intent);
        }  
    };
    View.OnClickListener listenerb = new OnClickListener() { 
    	@Override
    	public void onClick(View v) {
    		Intent intent = new Intent();  
            intent.setClass(MainActivity.this, Second.class);
            startActivity(intent);  //开始跳转 
    	};
    };
    
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
		if (requestCode == 1) {  
            if (resultCode == RESULT_OK) {  
                Toast.makeText(this, "蓝牙已经开启", Toast.LENGTH_SHORT).show();  
            } else if (resultCode == RESULT_CANCELED) {  
                Toast.makeText(this, "不允许蓝牙开启", Toast.LENGTH_SHORT).show();  
                finish();  
            }  
        }
		
	}

	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
}
