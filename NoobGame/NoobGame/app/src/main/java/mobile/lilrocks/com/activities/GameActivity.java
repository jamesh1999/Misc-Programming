package mobile.lilrocks.com.activities;

import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.provider.Settings;
import android.support.v7.app.AppCompatActivity;
import android.view.Window;
import android.view.WindowManager;
import android.widget.RelativeLayout;

import mobile.lilrocks.com.R;
import mobile.lilrocks.com.gameplay.GameController;
import mobile.lilrocks.com.graphics.MapView;
import mobile.lilrocks.com.graphics.SurfaceView;
import mobile.lilrocks.com.input.InputController;
import mobile.lilrocks.com.network.NetworkHandler;
import mobile.lilrocks.com.network.NetworkThread;

public class GameActivity extends AppCompatActivity
{

	SurfaceView gameRenderer;
	GameController controller;

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		//Set up fullscreen
		requestWindowFeature(Window.FEATURE_NO_TITLE);

		super.onCreate(savedInstanceState);

		getWindow().setFlags(
				WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

		//Create GLSurfaceView and bind to layout
		gameRenderer = new SurfaceView(this);

		setContentView(R.layout.activity_game);
		RelativeLayout layout = (RelativeLayout) findViewById(R.id.game);

		RelativeLayout.LayoutParams glParams = new RelativeLayout.LayoutParams(
				RelativeLayout.LayoutParams.MATCH_PARENT,
				RelativeLayout.LayoutParams.MATCH_PARENT);

		layout.addView(gameRenderer, glParams);

		controller = new GameController();
		controller.setRenderer(gameRenderer.getRenderer());

		long delta = 0;
		long last = System.nanoTime();

		while (true)
		{

			delta += System.nanoTime() - last;
			last = System.nanoTime();

			if(delta > 500000000)
			{
				delta -= 500000000;
				NetworkHandler.update();
				NetworkHandler.updateMap();
			}
		}
	}

	@Override
	protected void onPause() {
		super.onPause();
		gameRenderer.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
		gameRenderer.onResume();
	}
}
