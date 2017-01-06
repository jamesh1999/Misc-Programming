package mobile.lilrocks.com.network;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.Serializable;
import java.util.List;

/**
 * Created by james on 24/12/2016.
 */

public class ServerGame
{
	public long id;
	public String name;
	public boolean password;
	public boolean reserved;

	public static ServerGame fromString(String str)
	{
		String[] parts = str.substring(1, str.length() - 1).split(",");
		ServerGame ret = new ServerGame();
		ret.id = Long.parseLong(parts[0]);
		ret.name = parts[1];
		ret.password = Boolean.parseBoolean(parts[2]);
		ret.reserved = Boolean.parseBoolean(parts[3]);

		return ret;
	}
}
