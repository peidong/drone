package com.example.bluetooth;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Set;
import java.util.UUID;

import org.apache.http.conn.ConnectionReleaseTrigger;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;
import android.widget.Button;
import android.widget.ListAdapter;

public class MainActivity extends Activity implements OnItemClickListener {
	
	public static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
	protected static final int SUCCESS_CONNECT = 0;
	ArrayAdapter<String> listAdapter;
	ArrayList<String> pairedDevices;
	ListView listView;
	BluetoothAdapter btAdapter;
	Set<BluetoothDevice> devicesArray;
	IntentFilter filter;
	BroadcastReceiver receiver;
	Handler mHandler;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		init();
		if(btAdapter == null)
		{
			Toast.makeText(getApplicationContext(), "No bluetooth detected", 0).show();
			finish();
		}
		else
		{
			if(!btAdapter.isEnabled()){
				turnOnBT();
			}
			getPairedDevices();
			startDiscovery();
		}
		mHandler = new Handler(){
			@Override
			public void handleMessage(Message msg) {
				// TODO Auto-generated method stub
				super.handleMessage(msg);
				switch (msg.what) {
				case SUCCESS_CONNECT:
					//do sth
					//ConnectThread connectThread = new ConnectThread((BluetoothSocket)msg.obj);
					Toast.makeText(getApplicationContext(), "connected!!!!!", 0).show();
					break;

				}
			}
		};
	}

	private void startDiscovery() {
		// TODO Auto-generated method stub
		btAdapter.cancelDiscovery();
		btAdapter.startDiscovery();
		
	}

	private void turnOnBT() {
		// TODO Auto-generated method stub
		Intent intent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
		startActivityForResult(intent,1);
	}

	private void getPairedDevices() {
		// TODO Auto-generated method stub
		devicesArray = btAdapter.getBondedDevices();
		if(devicesArray.size()>0)
		{
			for(BluetoothDevice device:devicesArray){
				pairedDevices.add(device.getName()+"\n"+device.getAddress());
				
			}
		}
	}

	private void init() {
		// TODO Auto-generated method stub
        listView.setOnItemClickListener(this);
		listView=(ListView)findViewById(R.id.listView);
		listAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1,0);
		listView.setAdapter(listAdapter);
		btAdapter = BluetoothAdapter.getDefaultAdapter();
		pairedDevices = new ArrayList<String>();
		filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
		receiver = new BroadcastReceiver() {
			
			@Override
			public void onReceive(Context context, Intent intent) {
				// TODO Auto-generated method stub
				String action = intent.getAction();
				if(BluetoothDevice.ACTION_FOUND.equals(action)){
					BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
					
					
					String s = "";
					for(int i = 0; i < listAdapter.getCount(); i++)
					{
						for(int a = 0; a < pairedDevices.size(); a++)
						{
							if(listAdapter.getItem(i).equals(pairedDevices.get(a)))
							{
								
								s = "(PAIRED)";
								break;
							}
						}
						listAdapter.add(device.getName()+"\n"+device.getAddress());
					}
					
				}
				else if (BluetoothAdapter.ACTION_DISCOVERY_STARTED.equals(action)){
					//run some codes
				}
				else if (BluetoothAdapter.ACTION_DISCOVERY_FINISHED.equals(action)){
					//run some codes
					if(listAdapter.getCount()>0){
						
					}
				}
                else if (BluetoothAdapter.ACTION_STATE_CHANGED.equals(action)){
                	//run some codes
                	if(btAdapter.getState() == btAdapter.STATE_OFF){
                		turnOnBT();
                	}
				}
			}
		};
		registerReceiver(receiver, filter);
		 filter = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_STARTED);
		registerReceiver(receiver, filter);
		 filter = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
		registerReceiver(receiver, filter);
		 filter = new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED);
		registerReceiver(receiver, filter);
	}
	
	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
		unregisterReceiver(receiver);
	}

	@Override
	
	protected void onActivityResult(int requestCode, int resultCode, Intent data){
		super.onActivityResult(requestCode,resultCode,data);
		if(resultCode == RESULT_CANCELED){
			Toast.makeText(getApplicationContext(), "bluetooth must be enabled to continue", Toast.LENGTH_SHORT).show();
			finish();
		}
	}
	
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

	@Override
	public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
		// TODO Auto-generated method stub
		if(btAdapter.isDiscovering()){
			btAdapter.cancelDiscovery();
		}
		if(listAdapter.getItem(position).contains("(PAIRED)")){
			Toast.makeText(getApplicationContext(), "device is paired", 0).show();
			Object[] o = devicesArray.toArray();
			BluetoothDevice selectedDevice = (BluetoothDevice)o[position];
			ConnectThread connect = new ConnectThread(selectedDevice);
			connect.start();
		}
		else{
			Toast.makeText(getApplicationContext(), "device is not paired", 0).show();
		}
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	private class ConnectThread extends Thread {
	    
		private final BluetoothSocket mmSocket;
	    private final BluetoothDevice mmDevice;
	 
	    public ConnectThread(BluetoothDevice device) {
	        // Use a temporary object that is later assigned to mmSocket,
	        // because mmSocket is final
	        BluetoothSocket tmp = null;
	        mmDevice = device;
	 
	        // Get a BluetoothSocket to connect with the given BluetoothDevice
	        try {
	            // MY_UUID is the app's UUID string, also used by the server code
	            tmp = device.createRfcommSocketToServiceRecord(MY_UUID);
	        } catch (IOException e) { }
	        mmSocket = tmp;
	    }
	 
	    public void run() {
	        // Cancel discovery because it will slow down the connection
	        btAdapter.cancelDiscovery();
	 
	        try {
	            // Connect the device through the socket. This will block
	            // until it succeeds or throws an exception
	            mmSocket.connect();
	        } catch (IOException connectException) {
	            // Unable to connect; close the socket and get out
	            try {
	                mmSocket.close();
	            } catch (IOException closeException) { }
	            return;
	        }
	 
	        // Do work to manage the connection (in a separate thread)
	        manageConnectedSocket(mmSocket);
	        mHandler.obtainMessage(SUCCESS_CONNECT);
	    }
	 
	    private void manageConnectedSocket(BluetoothSocket mmSocket2) {
			// TODO Auto-generated method stub
			
		}

		/** Will cancel an in-progress connection, and close the socket */
	    public void cancel() {
	        try {
	            mmSocket.close();
	        } catch (IOException e) { }
	    }
	}
}
