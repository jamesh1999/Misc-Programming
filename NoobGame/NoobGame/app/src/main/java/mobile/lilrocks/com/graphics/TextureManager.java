package mobile.lilrocks.com.graphics;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.BitmapRegionDecoder;
import android.text.style.SubscriptSpan;

import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.Vector;

import mobile.lilrocks.com.Utils;

/**
 * Created by james on 02/01/2017.
 */

public class TextureManager
{
	private static HashMap<String, Integer> loadedIDs = new HashMap<>();
	private static HashMap<String, Bitmap> loadedImages = new HashMap<>();
	private static int idCnt = 0;

	private static Bitmap atlas = Bitmap.createBitmap(2048, 2048, Bitmap.Config.ARGB_8888);
	private static HashMap<Integer, Integer[]> positions = new HashMap<>();

	public static int loadTexture(String file, String dir)
	{
		try
		{
			Bitmap bmp = BitmapFactory.decodeStream(Utils.getAssetManager().open("textures/" + dir + "/" + file));
			loadedImages.put(dir + "/" + file, bmp);
			loadedIDs.put(dir + "/" + file, idCnt++);
			return idCnt;
		}
		catch (IOException e)
		{
			System.out.println(e);
			return -1;
		}
	}

	public static void unloadTexture(String file, String dir)
	{
		loadedImages.remove(dir + "/" + file);
		loadedIDs.remove(dir + "/" + file);
	}

	public static void buildAtlas()
	{
		positions.clear();
		Iterator<Map.Entry<String, Bitmap>> it = loadedImages.entrySet().iterator();

		int pos = 0;
		while (it.hasNext())
		{
			Map.Entry<String, Bitmap> entry = it.next();

			int[] data = new int[entry.getValue().getWidth() * entry.getValue().getHeight()];
			entry.getValue().getPixels(data, 0, 32, 0, 0, 32, 32);
			atlas.setPixels(data, 0, 32, 32 * (pos % 64), 32 * (pos / 64), 32, 32);

			Integer[] position = {pos % 64, pos / 64};
			positions.put(loadedIDs.get(entry.getKey()), position);
			++pos;
			it.remove();
		}
	}

	public static Bitmap getAtlas()
	{
		return atlas;
	}

	public static float[] getUV(int id)
	{
		Integer[] pos = positions.get(id);
		float[] uv = {pos[0] / 64.0f, pos[1] / 64.0f, (pos[0] + 1) / 64.0f, (pos[1] + 1) / 64.0f};
		return uv;
	}
}
