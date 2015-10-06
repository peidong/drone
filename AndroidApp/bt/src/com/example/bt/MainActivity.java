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
            // ����ͨ��startActivityForResult()���������Intent������onActivityResult()�ص������л�ȡ�û���ѡ�񣬱����û�������Yes������  
            // ��ô�����յ�RESULT_OK�Ľ����  
            // ���RESULT_CANCELED������û���Ը�⿪������  
            Intent mIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);  
            startActivityForResult(mIntent, 1);
            
            // ��enable()����������������ѯ���û�(ʵ������Ϣ�Ŀ��������豸),��ʱ����Ҫ�õ�android.permission.BLUETOOTH_ADMINȨ�ޡ�  
			//mAdapter.enable();  
            // mBluetoothAdapter.disable();//�ر�����  
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
        	//�����޸������ɼ��Ե�Intent
    		Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
    		//���������ɼ��Ե�ʱ�䣬��������涨���ɼ�300��
    		intent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 10);
    		startActivity(intent);
        }  
    };
    View.OnClickListener listenerb = new OnClickListener() { 
    	@Override
    	public void onClick(View v) {
    		Intent intent = new Intent();  
            intent.setClass(MainActivity.this, Second.class);
            startActivity(intent);  //��ʼ��ת 
    	};
    };
    
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
		if (requestCode == 1) {  
            if (resultCode == RESULT_OK) {  
                Toast.makeText(this, "�����Ѿ�����", Toast.LENGTH_SHORT).show();  
            } else if (resultCode == RESULT_CANCELED) {  
                Toast.makeText(this, "��������������", Toast.LENGTH_SHORT).show();  
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
