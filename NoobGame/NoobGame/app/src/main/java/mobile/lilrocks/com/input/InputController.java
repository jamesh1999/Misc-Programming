package mobile.lilrocks.com.input;

import android.view.MotionEvent;

import mobile.lilrocks.com.graphics.MapView;

/**
 * Created by james on 24/12/2016.
 */

public class InputController
{
	//Camera movement variables
	private static float[] previousCoords = {0.0f, 0.0f, 0.0f, 0.0f};
	private static boolean setPrevious = false;

	public static boolean onTouchEvent(MotionEvent e)
	{
		//Multi pointer = camera control
		if(e.getPointerCount() == 2)
		{
			MotionEvent.PointerCoords coords0 = new MotionEvent.PointerCoords();
			MotionEvent.PointerCoords coords1 = new MotionEvent.PointerCoords();
			e.getPointerCoords(0, coords0);
			e.getPointerCoords(1, coords1);

			if(setPrevious)
			{
				float deltaZoom = (float)(
						Math.sqrt(
							Math.pow(coords0.x - coords1.x, 2.0)
							+ Math.pow(coords0.y - coords1.y, 2.0))
						- Math.sqrt(
							Math.pow(previousCoords[0] - previousCoords[2], 2.0)
							+ Math.pow(previousCoords[1] - previousCoords[3], 2.0)));

				float[] centre = {
						(coords0.x + coords1.x) / 2,
						(coords0.y + coords1.y) / 2
				};

				float[] deltaTranslate = {
						centre[0] - (previousCoords[0] + previousCoords[2]) / 2,
						centre[1] - (previousCoords[1] + previousCoords[3]) / 2
				};

				cam.translate(deltaTranslate);
				cam.zoom(centre, deltaZoom);
			}

			previousCoords[0] = coords0.x;
			previousCoords[1] = coords0.y;

			previousCoords[2] = coords1.x;
			previousCoords[3] = coords1.y;

			setPrevious = true;

			return true;
		}
		else
			setPrevious = false;

		return false;
	}

	private static MapView cam;
	public static void registerCamera(MapView cc)
	{
		cam = cc;
	}
}
