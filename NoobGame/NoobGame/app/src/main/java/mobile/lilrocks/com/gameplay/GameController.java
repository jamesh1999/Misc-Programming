package mobile.lilrocks.com.gameplay;

import org.w3c.dom.Text;

import java.io.IOException;

import mobile.lilrocks.com.gameplay.assets.Manager;
import mobile.lilrocks.com.gameplay.assets.Terrain;
import mobile.lilrocks.com.gameplay.assets.Unit;
import mobile.lilrocks.com.gameplay.player.IPlayer;
import mobile.lilrocks.com.graphics.GameRenderer;
import mobile.lilrocks.com.graphics.MapView;
import mobile.lilrocks.com.graphics.TextureManager;
import mobile.lilrocks.com.input.InputController;

/**
 * Created by james on 24/12/2016.
 */

public class GameController
{
	IPlayer[] players;
	MapView camera;
	GameRenderer renderer;

	public static Manager<Unit> unitManager;
	public static Manager<Terrain> terrainManager;

	public GameController()
	{
		//Asset instantiation
		unitManager = new Manager<>(Unit.class);
		terrainManager = new Manager<>(Terrain.class);

		try
		{
			unitManager.loadAssets("items/units");
			terrainManager.loadAssets("items/terrain");
		}
		catch (IOException e)
		{
			System.out.println(e);
		}

		//Graphics instantiation
		camera = new MapView();
		InputController.registerCamera(camera);

		TextureManager.buildAtlas();

	}

	public boolean isOnline()
	{
		return players[0].isOnline() | players[1].isOnline();
	}

	public void setRenderer(GameRenderer r)
	{
		renderer = r;
		renderer.setCamera(camera);

		renderer.loadTex();

		World w = renderer.world;
		for(int i = 0; i < w.height; ++i)
			for(int j = 0; j < w.width; ++j)
			{
				w.terrain[i][j].refreshUV();

			}
	}
}
