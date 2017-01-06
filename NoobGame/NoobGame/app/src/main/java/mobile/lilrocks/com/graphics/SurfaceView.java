package mobile.lilrocks.com.graphics;


import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

import mobile.lilrocks.com.input.InputController;

/**
 * Created by james on 06/11/2016.
 */

public class SurfaceView extends GLSurfaceView
{
	private final GameRenderer renderer;

	public SurfaceView(Context context)
	{
		super(context);

		setEGLContextClientVersion(2);
		renderer = new GameRenderer(context);
		setRenderer(renderer);
		setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
	}

	@Override
	public void onPause()
	{
		super.onPause();
		renderer.onPause();
	}

	@Override
	public void onResume()
	{
		super.onResume();
		renderer.onResume();
	}

	@Override
	public boolean onTouchEvent(MotionEvent e)
	{
		return InputController.onTouchEvent(e);
	}

	public GameRenderer getRenderer()
	{
		return renderer;
	}
}
