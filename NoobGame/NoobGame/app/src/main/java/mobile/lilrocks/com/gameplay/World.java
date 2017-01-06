package mobile.lilrocks.com.gameplay;

import mobile.lilrocks.com.gameplay.assets.Terrain;
import mobile.lilrocks.com.gameplay.assets.Unit;

/**
 * Created by james on 04/12/2016.
 */

public class World
{
	public Terrain[][] terrain;
	public Unit[][] units;

	public int width;
	public int height;

	public void unitsFromString(String data)
	{

	}

	public void terrainFromString(String data)
	{
		String[] rows = data.split("\n");
		height = rows.length;

		terrain = new Terrain[height][];

		for(int i = 0; i < height; ++i)
		{
			String[] squares = rows[i].split(",");
			width = squares.length;

			terrain[i] = new Terrain[width];

			for(int j = 0; j < width; ++j)
			{
				terrain[i][j] = GameController.terrainManager.getAsset(squares[j]);
				float[] pos = {(float)i, (float)j};
				terrain[i][j].SetPosition(pos);
			}
		}
	}
}
