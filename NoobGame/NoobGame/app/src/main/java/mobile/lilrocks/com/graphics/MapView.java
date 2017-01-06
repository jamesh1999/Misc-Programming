package mobile.lilrocks.com.graphics;

import mobile.lilrocks.com.Utils;

/**
 * Created by james on 24/12/2016.
 */

public class MapView
{
	private float[] position;
	private float zoom;
	private float aspect;

	public MapView()
	{
		position = new float[2];
		resetCamera();
	}

	public void setAspect(float aspectRatio)
	{
		aspect = aspectRatio;
	}

	public void resetCamera()
	{
		position[0] = 0.5f;
		position[1] = 0.5f;
		zoom = 1.0f;
	}

	//Camera movement controls
	public void zoom(float[] centre, float delta)
	{
		delta = Utils.clamp(delta, 1.0f - zoom, 8.0f - zoom);

		float multiplier = 1 + delta / zoom;
		zoom += delta;

		float[] offset = new float[2];
		offset[0] = (1 - multiplier) * centre[0];
		offset[1] = (1 - multiplier) * centre[1];
		translate(offset);
	}

	public void translate(float[] delta)
	{
		float ygap = 0.5f / zoom;
		float xgap = ygap * aspect;

		position[0] = Utils.clamp(position[0] + delta[0], xgap, 1.0f - xgap);
		position[1] = Utils.clamp(position[1] + delta[1], ygap, 1.0f - ygap);
	}

	//Accessors for Renderer
	public float[] getPosition()
	{
		return position;
	}
	public float getZoom()
	{
		return zoom;
	}
}
