package mobile.lilrocks.com.graphics;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLES20;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * Created by james on 06/11/2016.
 */

public class GraphicsUtils
{
	public static int glProgram;

	public static int LoadShaderFromFile(Context context, int type, String filename) throws IOException
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
			line = reader.readLine();
		}

		in.close();

		return LoadShaderFromString(type, builder.toString());
	}

	public static int LoadShaderFromString(int type, String source)
	{
		int s = GLES20.glCreateShader(type);

		GLES20.glShaderSource(s, source);
		GLES20.glCompileShader(s);

		return s;
	}
}
