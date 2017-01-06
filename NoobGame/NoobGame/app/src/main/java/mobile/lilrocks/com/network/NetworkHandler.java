package mobile.lilrocks.com.network;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import mobile.lilrocks.com.Utils;
import mobile.lilrocks.com.gameplay.World;

/**
 * Created by james on 27/10/2016.
 */

public class NetworkHandler
{
	private NetworkHandler(){}

	public static ArrayList<String> notices = new ArrayList<String>();
	public static void validate(String key, int version)
	{
		StringBuilder sb = new StringBuilder();
		sb.append("valid|");
		sb.append(key);
		sb.append("|");
		sb.append(version);
		NetworkThread.writeMessage(sb.toString());
	}

	public static void update()
	{
		while(true)
		{
			String msg = NetworkThread.readMessage();
			if(msg == "") break;

			//Handle notices
			if(msg.startsWith("notice|"))
				notices.add(msg.substring(7));

			//Ping replies
			else if(msg.startsWith("ping|"))
				ping = Float.parseFloat(msg.substring(5));

			//Add games
			else if(msg.startsWith("gamelist|"))
				games.add(ServerGame.fromString(msg.substring(9)));

			//Update map buffer
			else if(msg.startsWith("mapstring|"))
				map.unitsFromString(msg.substring(10));
			else if(msg.startsWith("enteredgame|"))
			{
				String[] parts = msg.split("\\|");

				try
				{
					map.terrainFromString(Utils.loadResource(parts[2]));
				}
				catch(IOException e)
				{
					System.err.print(e);
				}
			}
		}
	}

	private static float ping;
	public static void refreshPing()
	{
		NetworkThread.writeMessage("ping");
	}
	public static float lastPing()
	{
		return ping;
	}

	public static ArrayList<ServerGame> games = new ArrayList<ServerGame>();
	public static void getAllGames()
	{
		games.clear();
		NetworkThread.writeMessage("getallgames");
	}
	public static void getOpenGames()
	{
		games.clear();
		NetworkThread.writeMessage("getgames");
	}

	public static void enter(Long id, String password)
	{
		StringBuilder sb = new StringBuilder();
		sb.append("entergame|");
		sb.append(id);
		sb.append("|");
		sb.append(password);
		NetworkThread.writeMessage(sb.toString());
	}

	public static void join(Long id, String password, String gamePassword)
	{
		StringBuilder sb = new StringBuilder();
		sb.append("joingame|");
		sb.append(id);
		sb.append("|");
		sb.append(password);
		if(gamePassword != null && !gamePassword.isEmpty())
		{
			sb.append("|");
			sb.append(gamePassword);
		}
		NetworkThread.writeMessage(sb.toString());
	}

	public static void create(String name, String map, int res, String password, String gamePassword)
	{
		StringBuilder sb = new StringBuilder();
		sb.append("creategame|");
		sb.append(name);
		sb.append("|");
		sb.append(map);
		sb.append("|");
		sb.append(res);
		sb.append("|");
		sb.append(password);
		if(gamePassword != null && !gamePassword.isEmpty())
		{
			sb.append("|");
			sb.append(gamePassword);
		}
		NetworkThread.writeMessage(sb.toString());
	}

	public static World map = new World();
	public static void updateMap()
	{
		NetworkThread.writeMessage("viewmap");
	}
}
