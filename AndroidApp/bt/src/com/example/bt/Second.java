package com.example.bt;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Set;
import java.util.UUID;



import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;
import android.widget.Adapter;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;
import android.view.View.OnClickListener;
import android.widget.Toast;

public class Second extends Activity {
	
	private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");
	BluetoothAdapter mAdapter;
	Set<BluetoothDevice> deviceArray = null;
	BluetoothDevice pairedDevice;
	Button mButton;
	public int pos_in_bondedDevice;
	private TextView text;
	private ListView listView1,listView2;
	private ArrayList<String> mDeviceList1 = new ArrayList<String>();
	private ArrayList<String> mDeviceList2 = new ArrayList<String>();
	
	private BroadcastReceiver mReceiver = new BroadcastReceiver() {
		public void onReceive(android.content.Context context, Intent intent) {
			String action = intent.getAction();
			//获取设备
			if (action.equals(BluetoothDevice.ACTION_FOUND)) {  
                BluetoothDevice device = intent  
                        .getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);  
                // 搜索到的不是已经绑定的蓝牙设备  
                if (device.getBondState() == BluetoothDevice.BOND_BONDED) {  
                    // 显示在TextView上  
                	//text.append(device.getName() + ":"  
                    //        + device.getAddress()+"\n");  
                	
                	mDeviceList1.add("Paired device:" + "\n" +device.getName() 
                	+ ": " + device.getAddress()+ "\n");
                	//Log.i("BT", device.getName() + device.getAddress() + "\n");
                	listView1.setAdapter(new ArrayAdapter<String>(context,
                            android.R.layout.simple_list_item_1, mDeviceList1));
                	pairedDevice = device;
                	
                	
                }
                else
                {
                	mDeviceList2.add("Unpaired device:" + "\n" +device.getName() 
                	+ ": " + device.getAddress()+ "\n");
                	//Log.i("BT", device.getName() + device.getAddress() + "\n");
                	listView2.setAdapter(new ArrayAdapter<String>(context,
                            android.R.layout.simple_list_item_1, mDeviceList2));
                }
                // 搜索完成  
            } else if (action.equals(BluetoothAdapter.ACTION_DISCOVERY_FINISHED)) {  
                setProgressBarIndeterminateVisibility(false);  
                setTitle("搜索蓝牙设备");  
            }
			
			listView1.setOnItemClickListener(itemClickListener1);
			listView2.setOnItemClickListener(itemClickListener2);
			
			
		}
	};
	
	public class ConnectThread extends Thread {
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
	        mAdapter.cancelDiscovery();
	 
	        try {
	            // Connect the device through the socket. This will block
	            // until it succeeds or throws an exception
	            mmSocket.connect();
	        } catch (IOException connectException) {
	            // Unable to connect; close the socket and get out
	            try {
	                mmSocket.close();
	            } catch (IOException closeException) { }
	            
	            Intent intent = new Intent();  
	            intent.setClass(Second.this, Third.class);
	            startActivity(intent);  //开始跳转，如果跳转过去后中途断开蓝牙连接，如何操作
	            
	            return;
	        }
	 
	        // Do work to manage the connection (in a separate thread)
	        //manageConnectedSocket(mmSocket);
	    }
	 

		/** Will cancel an in-progress connection, and close the socket */
	    public void cancel() {
	        try {
	            mmSocket.close();
	        } catch (IOException e) { }
	    }
	}
	
	OnItemClickListener itemClickListener1 = new OnItemClickListener() {
        @Override
        public void onItemClick(AdapterView<?> parent, View container, int position, long id) {
            // Getting the Container Layout of the ListView
        	
        	mAdapter.cancelDiscovery();
        	
        	
        	deviceArray = mAdapter.getBondedDevices();

        	Object[] o = deviceArray.toArray();
        	for(int i = 0; i < deviceArray.size();i++)
        	{
        		if(mDeviceList1.get(position).contains(o[i].toString()))
        		{
        			pos_in_bondedDevice = i;
        			break;
        		}
        	}
        	
        	BluetoothDevice selectedDevice = (BluetoothDevice)o[pos_in_bondedDevice];
			
			ConnectThread connect = new ConnectThread(selectedDevice);
			connect.start();
        	
			Toast.makeText(getApplicationContext(), "this is paired, you can connect "+o[pos_in_bondedDevice], 0).show();
        	//try to connect
			
        }
    };
    OnItemClickListener itemClickListener2 = new OnItemClickListener() {
        @Override
        public void onItemClick(AdapterView<?> parent, View container, int position, long id) {
            // Getting the Container Layout of the ListView
        	mAdapter.cancelDiscovery();
        	Toast.makeText(getApplicationContext(), "this is unpaired, you need to pair first", 0).show();
        }
    };
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.sec);
		
		
        
        mButton = (Button) this.findViewById(R.id.c);  
        mButton.setOnClickListener(listenerc);
         
        
        
	}
	
	
	
	@Override  
    protected void onDestroy() {  
        // TODO Auto-generated method stub  
        super.onDestroy();  
        //解除注册  
        unregisterReceiver(mReceiver);  
        //System.exit(0);
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

	View.OnClickListener listenerc = new OnClickListener() {	
	public void onClick(View v) {
		
		//text = (TextView) findViewById(R.id.textView);
		//text.setText("");
		listView1 = (ListView) findViewById(R.id.lv1);
		mDeviceList1.clear();
		listView1.setAdapter(null);
		listView2 = (ListView) findViewById(R.id.lv2);
		mDeviceList2.clear();
		listView2.setAdapter(null);
		mAdapter= BluetoothAdapter.getDefaultAdapter();
        //setProgressBarIndeterminateVisibility(true);  
        setTitle("正在扫描....");  
        // 如果正在搜索，就先取消搜索  
        if (mAdapter.isDiscovering()) {  
        	mAdapter.cancelDiscovery();  
        }  
        // 开始搜索蓝牙设备,搜索到的蓝牙设备通过广播返回  
        mAdapter.startDiscovery();
        
		
		//Set<BluetoothDevice> devices = mAdapter.getBondedDevices();
		//if(devices.size()>0)
		//{
		//	for (BluetoothDevice bluetoothDevice : devices) {  
        //        //text.append(bluetoothDevice.getName() + ":"  
        //        //        + bluetoothDevice.getAddress() + "\n\n"); 
		//		mDeviceList.add(bluetoothDevice.getName() + ":"  
        //                + bluetoothDevice.getAddress() + "\n\n" + "this appear first");
        //    }
		//}
		// 注册用以接收到已搜索到的蓝牙设备的receiver  
        IntentFilter mFilter = new IntentFilter(BluetoothDevice.ACTION_FOUND);  
        registerReceiver(mReceiver, mFilter);  
        // 注册搜索完时的receiver  
        mFilter = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);  
        registerReceiver(mReceiver, mFilter);
    }
	
	};
}
