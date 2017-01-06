package mobile.lilrocks.com.graphics;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.opengl.GLUtils;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;
import java.util.ArrayList;
import java.util.List;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import mobile.lilrocks.com.gameplay.World;

/**
 * Created by james on 06/11/2016.
 */

public class GameRenderer implements GLSurfaceView.Renderer
{
	public World world = new World();
	MapView camera;

	private static ArrayList<Sprite> sprites = new ArrayList<>();

	//P, V & VP matrices
	private final float[] mtrxP = new float[16];
	private final float[] mtrxV = new float[16];
	private final float[] mtrxVP = new float[16];

	//Buffers
	public static float vertices[];
	public static short indices[];
	public static float uvs[];
	public FloatBuffer vertexBuffer;
	public ShortBuffer drawListBuffer;
	public FloatBuffer uvBuffer;


	float mScreenWidth = 1280;
	float mScreenHeight = 768;

	// Misc
	Context mContext;
	long mLastTime;
	int mProgram;

	public GameRenderer(Context c)
	{
		mContext = c;
		mLastTime = System.currentTimeMillis() + 100;
	}

	boolean reload = false;
	public void loadTex()
	{
		reload = true;
	}

	public void onPause()
	{
        /* Do stuff to pause the renderer */
	}

	public void onResume()
	{
        /* Do stuff to resume the renderer */
		mLastTime = System.currentTimeMillis();
	}

	public void setCamera(MapView cam)
	{
		cam.setAspect(mScreenWidth / mScreenHeight);
		camera = cam;
	}

	@Override
	public void onDrawFrame(GL10 gl)
	{
		// Get the current time
		long now = System.currentTimeMillis();

		// We should make sure we are valid and sane
		if (mLastTime > now) return;

		// Get the amount of time the last frame took.
		long elapsed = now - mLastTime;

		// Update our example
		Matrix.rotateM(mtrxV, 0, mtrxV, 0, 0.05f * elapsed, 0.0f, 0.0f, 1.0f);
		System.out.println("Rotated: " + String.valueOf(0.02f * elapsed / 10.0f));
		Matrix.multiplyMM(mtrxVP, 0, mtrxP, 0, mtrxV, 0);

		// Render our example
		Render(mtrxVP);

		// Save the current time to see how long it took <img src="http://androidblog.reindustries.com/wp-includes/images/smilies/icon_smile.gif" alt=":)" class="wp-smiley"> .
		mLastTime = now;

	}

	private void Render(float[] m)
	{
		if(reload)
		{
			int[] texturenames = new int[1];
			GLES20.glGenTextures(1, texturenames, 0);

			GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, texturenames[0]);

			// Set filtering
			GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
			GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);

			// Load the bitmap into the bound texture.
			GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, TextureManager.getAtlas(), 0);
			reload = false;
		}

		ByteBuffer bb = ByteBuffer.allocateDirect(sprites.size() * 16);
		bb.order(ByteOrder.nativeOrder());
		vertexBuffer = bb.asFloatBuffer();

		ByteBuffer dlb = ByteBuffer.allocateDirect(sprites.size() * 12);
		dlb.order(ByteOrder.nativeOrder());
		drawListBuffer = dlb.asShortBuffer();

		ByteBuffer uvb = ByteBuffer.allocateDirect(sprites.size() * 32);
		uvb.order(ByteOrder.nativeOrder());
		uvBuffer = uvb.asFloatBuffer();

		for(int i = 0; i < sprites.size(); ++i)
			GenerateQuad(sprites.get(i));

		vertexBuffer.position(0);
		drawListBuffer.position(0);
		uvBuffer.position(0);

		//Calculate camera matrices
		float[] pos = camera.getPosition();
		float zoom = camera.getZoom();
		Matrix.setLookAtM(mtrxV, 0, pos[0], pos[1], 1f, pos[0], pos[1], 0f, 0f, 1.0f, 0.0f);
		Matrix.scaleM(mtrxV, 0, zoom, zoom, zoom);
		Matrix.multiplyMM(mtrxVP, 0, mtrxP, 0, mtrxV, 0);

		// clear Screen and Depth Buffer, we have set the clear color as black.
		GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);

		// get handle to vertex shader's vPosition member
		int mPositionHandle = GLES20.glGetAttribLocation(GraphicsUtils.glProgram, "vPosition");

		// Enable generic vertex attribute array
		GLES20.glEnableVertexAttribArray(mPositionHandle);

		// Prepare the triangle coordinate data
		GLES20.glVertexAttribPointer(mPositionHandle, 3,
				GLES20.GL_FLOAT, false,
				0, vertexBuffer);

		// Get handle to texture coordinates location
		int mTexCoordLoc = GLES20.glGetAttribLocation(GraphicsUtils.glProgram,
				"a_texCoord" );

		// Enable generic vertex attribute array
		GLES20.glEnableVertexAttribArray ( mTexCoordLoc );

		// Prepare the texturecoordinates
		GLES20.glVertexAttribPointer ( mTexCoordLoc, 2, GLES20.GL_FLOAT,
				false,
				0, uvBuffer);

		// Get handle to shape's transformation matrix
		int mtrxhandle = GLES20.glGetUniformLocation(GraphicsUtils.glProgram, "uMVPMatrix");

		// Apply the projection and view transformation
		GLES20.glUniformMatrix4fv(mtrxhandle, 1, false, m, 0);

		// Get handle to textures locations
		int mSamplerLoc = GLES20.glGetUniformLocation (GraphicsUtils.glProgram, "s_texture" );

		// Set the sampler texture unit to 0, where we have saved the texture.
		GLES20.glUniform1i ( mSamplerLoc, 0);

		// Draw the triangle
		GLES20.glDrawElements(GLES20.GL_TRIANGLES, indices.length,
				GLES20.GL_UNSIGNED_SHORT, drawListBuffer);

		// Disable vertex array
		GLES20.glDisableVertexAttribArray(mPositionHandle);

	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height)
	{

		// We need to know the current width and height.
		mScreenWidth = width;
		mScreenHeight = height;

		// Redo the Viewport, making it fullscreen.
		GLES20.glViewport(0, 0, (int)mScreenWidth, (int)mScreenHeight);

		// Clear our matrices
		for(int i=0;i<16;i++)
		{
			mtrxP[i] = 0.0f;
			mtrxV[i] = 0.0f;
			mtrxVP[i] = 0.0f;
		}

		// Setup our screen width and height for normal sprite translation.
		float aspect = mScreenWidth / mScreenHeight;
		Matrix.orthoM(mtrxP, 0, -aspect, aspect, -1.0f, 1.0f, 0, 50);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config)
	{

		// Set the clear color to black
		GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1);

		GraphicsUtils.glProgram = GLES20.glCreateProgram();

		// Create the shaders
		try
		{
			int vertexShader = GraphicsUtils.LoadShaderFromFile(mContext, GLES20.GL_VERTEX_SHADER, "vtx.glsl");
			int fragmentShader = GraphicsUtils.LoadShaderFromFile(mContext, GLES20.GL_FRAGMENT_SHADER, "frag.glsl");

			GLES20.glAttachShader(GraphicsUtils.glProgram, vertexShader);   // add the vertex shader to program
			GLES20.glAttachShader(GraphicsUtils.glProgram, fragmentShader); // add the fragment shader to program
		}
		catch(IOException e)
		{
			System.err.println("Failed to load shaders");
		}

		GLES20.glLinkProgram(GraphicsUtils.glProgram);
		GLES20.glUseProgram(GraphicsUtils.glProgram);
	}

	public void GenerateQuad(Sprite s)
	{
		float[] centre = s.GetPosition();
		float[] extents = s.GetScale();
		// We have create the vertices of our view.
		vertices = new float[]
				{centre[0] - extents[0] / 2, centre[1] - extents[1] / 2, 0.0f,
						centre[0] - extents[0] / 2, centre[1] + extents[1] / 2, 0.0f,
						centre[0] + extents[0] / 2, centre[1] + extents[1] / 2, 0.0f,
						centre[0] + extents[0] / 2, centre[1] - extents[1] / 2, 0.0f,
				};

		indices = new short[] {0, 1, 2, 0, 2, 3}; // loop in the android official tutorial opengles why different order.

		float[] sprite = s.getUV();

		uvs = new float[]
				{
					sprite[0], sprite[1],
						sprite[0], sprite[3],
						sprite[2], sprite[3],
						sprite[2], sprite[1]
				};

		// The vertex buffer.
		vertexBuffer.put(vertices);

		// initialize byte buffer for the draw list
		drawListBuffer.put(indices);

		uvBuffer.put(uvs);
	}

	public static void registerSprite(Sprite s)
	{
		sprites.add(s);
	}

	public static void freeSprite(Sprite s)
	{
		sprites.remove(s);
	}
}
