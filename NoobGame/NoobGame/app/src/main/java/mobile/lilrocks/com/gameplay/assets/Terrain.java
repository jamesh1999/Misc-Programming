package mobile.lilrocks.com.gameplay.assets;

import java.util.ArrayList;
import java.util.HashMap;

import mobile.lilrocks.com.graphics.Sprite;
import mobile.lilrocks.com.graphics.TextureManager;

/**
 * Created by james on 04/12/2016.
 */

public class Terrain extends Sprite implements Asset
{
	//Data
	String terrainName = new String();
	float bonusDefenseValue;
	boolean capturable;
	HashMap<String, Integer> unitTypeCost = new HashMap<>();
	int texID;

	@Override
	public void fromString(String x)
	{
		String[] lines = x.split("\n");
		HashMap<String, String> kvps = new HashMap<>();

		for(int i = 0; i < lines.length; ++i)
		{
			String[] parts = (lines[i]+" ").split("=");
			if(parts.length < 2) continue;

			if(parts[1].trim() == null)
				parts[1] = "";
			else
				parts[1] = parts[1].trim();
			kvps.put(parts[0].trim(), parts[1]);
		}

		terrainName = kvps.get("terrainName");
		bonusDefenseValue = Float.parseFloat(kvps.get("bonusDefenseValue"));
		capturable = Boolean.parseBoolean(kvps.get("capturable"));

		boolean toSplit = true;
		char[] working = kvps.get("unitTypeCost").toCharArray();
		StringBuilder sb = new StringBuilder();
		ArrayList<String> unitData = new ArrayList<>();
		for(int i = 0; i < working.length; ++i)
		{
			if(working[i] == '[') toSplit = false;
			else if(working[i] == ']') toSplit = true;
			else if(working[i] == ',' && toSplit)
			{
				unitData.add(sb.toString());
				sb.setLength(0);
			}
			else
				sb.append(working[i]);
		}
		unitData.add(sb.toString());

		for(int i = 0; i < unitData.size(); ++i)
		{
			String[] parts = unitData.get(i).split(",");
			unitTypeCost.put(parts[0].trim(), Integer.parseInt(parts[1]));
		}

		texID = TextureManager.loadTexture(kvps.get("image"), "terrain");
	}

	@Override
	public String getKey()
	{
		return terrainName;
	}

	@Override
	public void refreshUV()
	{
		setUV(TextureManager.getUV(texID));
	}
}
