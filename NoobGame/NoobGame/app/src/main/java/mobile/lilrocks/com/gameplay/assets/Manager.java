package mobile.lilrocks.com.gameplay.assets;

import java.io.IOException;
import java.util.HashMap;

import mobile.lilrocks.com.Utils;


/**
 * Created by james on 02/01/2017.
 */

public class Manager<AssetType extends Asset>
{
	private HashMap<String, AssetType> assets = new HashMap<>();
	private Class<AssetType> classRef;

	public Manager(Class<AssetType> ref)
	{
		classRef = ref;
	}

	public AssetType getAsset(String key)
	{
		return assets.get(key);
	}

	public void loadAssets(String dir) throws IOException
	{
		try
		{
			String[] files = Utils.getAssetManager().list(dir);
			for (int i = 0; i < files.length; ++i)
			{
				AssetType n;
				n = classRef.newInstance();
				n.fromString(Utils.loadResource(dir + "/" + files[i]));
				assets.put(n.getKey(), n);
			}
		}
		catch(IllegalAccessException e)
		{
			System.out.println(e);
		}
		catch(InstantiationException e)
		{
			System.out.println(e);
		}
	}
}
