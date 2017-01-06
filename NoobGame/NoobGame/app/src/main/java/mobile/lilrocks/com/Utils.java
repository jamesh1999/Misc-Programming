package mobile.lilrocks.com;

import android.content.Context;
import android.content.res.AssetManager;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * Created by james on 24/12/2016.
 */

public class Utils
{
	private Utils() {}

	private static Context context;

	public static void setContext(Context c)
	{
		context = c;
	}

	public static AssetManager getAssetManager() { return context.getAssets(); }

	public static float clamp(float val, float min, float max)
	{
		return Math.min(max, Math.max(min, val));
	}

	public static String loadResource(String filename) throws IOException
	{
		//Open file
		AssetManager am = context.getAssets();
		InputStream in = am.open(filename);
		BufferedReader reader =
				new BufferedReader(
						new InputStreamReader(in));

		//Read file contents
		StringBuilder builder = new StringBuilder();
		String line = reader.readLine();
		while(line != null)
		{
			builder.append(line);
			builder.append('\n');
			line = reader.readLine();
		}

		in.close();

		return builder.toString();
	}
}
