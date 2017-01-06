package mobile.lilrocks.com.gameplay.assets;

/**
 * Created by james on 02/01/2017.
 */

public interface Asset
{
	void fromString(String x);
	String getKey();
	void refreshUV();
}
