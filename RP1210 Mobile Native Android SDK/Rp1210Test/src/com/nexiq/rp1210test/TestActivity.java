package com.nexiq.rp1210test;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;



import android.app.Activity;
import android.app.Fragment;
import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

public class TestActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_test);
		if (savedInstanceState == null) {
			getFragmentManager().beginTransaction().add(R.id.container, new PlaceholderFragment()).commit();
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.test, menu);
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

	/**
	 * A placeholder fragment containing a simple view.
	 */
	public static class PlaceholderFragment extends Fragment {

		public Rp1210Lib mRp1210Library = new Rp1210Lib();
		private Button mRp1210TestButton;
		
		public TextView mUiFramesPerSec;
		public TextView mUiDroppedFrames;

		public Context ctx;
		public int copied_file_len;
		public Rp1210TestThread test_thread;
		
		public PlaceholderFragment() {
		}

		@Override
		public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
			this.ctx = getActivity();
			View rootView = inflater.inflate(R.layout.fragment_test, container, false);

			try {
				String[] files = ctx.getAssets().list("Files");
				
				for(int i = 0; i < files.length; i++) 
				{
					CopyFileToDataPath(files[i]);
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
			
			
			mRp1210TestButton = (Button)rootView.findViewById(R.id.uiRp1210Test);
			mRp1210TestButton.setOnClickListener(new View.OnClickListener() {
				
				@Override
				public void onClick(View v) {
					onRp1210TestClick();
				}
			});
			
			return rootView;
		}
		
		private void onRp1210TestClick()
		{
			
			test_thread = new Rp1210TestThread(this);
			mRp1210TestButton.setEnabled(false);
			test_thread.start();
	
		}
		
		private void CopyFileToDataPath(String fname)
		{
			// copy compressed file from apk assets folder to private data folder for read/write in C/C++
			InputStream input;
			try {
				String path_fname = "Files/" + fname;
				input = ctx.getAssets().open(path_fname);
				
				int size = input.available();
				byte[] buffer = new byte[size];
				input.read(buffer);
				input.close();
				
				FileOutputStream outputStream;
				try{
					outputStream = ctx.openFileOutput(fname, Context.MODE_PRIVATE);
					outputStream.write(buffer);
					outputStream.close();
				}
				catch (Exception e){
					e.printStackTrace();
				}				
			} catch (IOException e)
			{
				e.printStackTrace();
			}	

		}
		
		public class Rp1210TestThread extends Thread {
			
			private PlaceholderFragment frag;
			
			Rp1210TestThread(PlaceholderFragment frag) {
				this.frag = frag;
			}
			
			public void run() {
				frag.mRp1210Library.Rp1210Test(ctx.getFilesDir().getPath());
				
				getActivity().runOnUiThread(new Runnable() {
				    public void run() {
				    	
				    	if(frag != null)
				    	{
				    		if(frag.mRp1210TestButton != null)
				    		{
				    			frag.mRp1210TestButton.setEnabled(true);		
				    		}
				    	}
				    	
				    }
				});
				
			}
		}
	}
}
