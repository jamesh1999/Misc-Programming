package mobile.lilrocks.com.graphics;

import android.opengl.Matrix;

/**
 * Created by james on 04/12/2016.
 */

public class Sprite
{
	private float[] pos;
	private float rot;
	private float[] scale;
	private float[] uv;

	public Sprite()
	{
		pos = new float[3];
		pos[0] = 0.0f;
		pos[1] = 0.0f;
		pos[2] = 0.0f;

		rot = 0.0f;

		scale = new float[2];
		scale[0] = 0.0f;
		scale[1] = 0.0f;

		uv = new float[4];
		uv[0] = 0.0f;
		uv[1] = 0.0f;
		uv[2] = 0.0f;
		uv[3] = 0.0f;

		GameRenderer.registerSprite(this);
	}

	public void delete()
	{
		GameRenderer.freeSprite(this);
	}

	public final void SetPosition(float[] npos)
	{
		pos[0] = npos[0];
		pos[1] = npos[1];
		pos[2] = npos[2];
	}

	public final void SetRotation(float nrot)
	{
		rot = nrot;
	}

	public final void SetScale(float[] nscale)
	{
		scale[0] = nscale[0];
		scale[1] = nscale[1];
	}

	public final void setUV(float[] nUV)
	{
		uv[0] = nUV[0];
		uv[1] = nUV[1];
		uv[2] = nUV[2];
		uv[3] = nUV[3];
	}

	public final float[] getUV()
	{
		return uv;
	}

	public final float[] GetPosition()
	{
		return pos;
	}

	public final float GetRotation()
	{
		return rot;
	}

	public final float[] GetScale()
	{
		return scale;
	}

	public final float[] GetTransform()
	{
		float[] mat = new float[16];
		Matrix.setIdentityM(mat, 0);
		Matrix.scaleM(mat, 0, scale[0], scale[1], scale[2]);
		Matrix.rotateM(mat, 0, rot, 0.0f, 0.0f, 1.0f);
		Matrix.translateM(mat, 0, pos[0], pos[1], pos[2]);
		return mat;
	}


}
