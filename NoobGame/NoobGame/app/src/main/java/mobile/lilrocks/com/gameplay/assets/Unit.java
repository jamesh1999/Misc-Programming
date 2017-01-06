package mobile.lilrocks.com.gameplay.assets;

import java.util.HashMap;

import mobile.lilrocks.com.graphics.Sprite;
import mobile.lilrocks.com.graphics.TextureManager;

/**
 * Created by james on 04/12/2016.
 */

public class Unit extends Sprite implements Asset
{
	//Data
	String unitName = new String();
	int baseCost;
	int baseSight;
	int baseHP;
	int baseMaxFuel;
	int baseMovement;
	String unitType = new String();
	String[] activeAblities = new String[0];
	String[] passiveAbilities = new String[0];
	int weaponSlots;
	int maxWeight;
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

		unitName = kvps.get("unitName");
		baseCost = Integer.parseInt(kvps.get("baseCost"));
		baseSight = Integer.parseInt(kvps.get("baseSight"));
		baseHP = Integer.parseInt(kvps.get("baseHP"));
		baseMaxFuel = Integer.parseInt(kvps.get("baseMaxFuel"));
		baseMovement = Integer.parseInt(kvps.get("baseMovement"));
		unitType = kvps.get("unitType");
		activeAblities = new String[0];//kvps.get("activeAbilities[]").split(",");
		passiveAbilities = new String[0]; //kvps.get("passiveAbilities[]").split(",");
		weaponSlots = Integer.parseInt(kvps.get("weaponSlots"));
		maxWeight = Integer.parseInt(kvps.get("maxWeight"));
		texID = TextureManager.loadTexture(kvps.get("image"), "units");
	}

	@Override
	public String getKey()
	{
		return unitName;
	}

	@Override
	public void refreshUV()
	{
		setUV(TextureManager.getUV(texID));
	}
}
