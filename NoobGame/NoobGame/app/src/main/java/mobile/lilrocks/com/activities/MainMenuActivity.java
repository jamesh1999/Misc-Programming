package mobile.lilrocks.com.activities;

import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ConfigurationInfo;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import mobile.lilrocks.com.R;
import mobile.lilrocks.com.Utils;
import mobile.lilrocks.com.network.NetworkThread;

public class MainMenuActivity extends AppCompatActivity
{
	private Thread networkingThread;

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		//Check OpenGL support
		ActivityManager am = (ActivityManager)
				getSystemService(Context.ACTIVITY_SERVICE);
		ConfigurationInfo info = am.getDeviceConfigurationInfo();
		if (info.reqGlEsVersion < 0x20000)
			System.exit(-1);

		setContentView(R.layout.activity_main);

		Utils.setContext(this);
	}

	public void goOnline(View view)
	{
		networkingThread = new Thread(NetworkThread.getInstance());
		networkingThread.start();

		Intent intent = new Intent(this, ServerSelectActivity.class);
		startActivity(intent);
	}
}