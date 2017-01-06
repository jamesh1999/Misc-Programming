package mobile.lilrocks.com.activities;

import android.os.Handler;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;

import java.util.ArrayList;

import mobile.lilrocks.com.R;
import mobile.lilrocks.com.network.NetworkHandler;
import mobile.lilrocks.com.network.NetworkThread;

public class ServerSelectActivity extends AppCompatActivity
{
int selectedGame = -1;
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_server_select);
		adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, games);
		ListView gameList = (ListView) findViewById(R.id.listView);
		gameList.setAdapter(adapter);

		gameList.setOnItemClickListener(new AdapterView.OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> parent, View view, int position,long arg3) {
				view.setSelected(true);
				selectedGame = position;
			}
		});
	}

	public void connect(View view)
	{
		EditText addressField = (EditText) findViewById(R.id.editText);
		String addr = addressField.getText().toString();
		System.err.print(addr);
		String[] parts = addr.split(":");
		NetworkThread.connect(parts[0], Integer.parseInt(parts[1]));
		NetworkHandler.validate("RWXQSHqaVw341ko5PHuBcYcXTHG0ewXd5QHksF71ZJi1etsq4KiwdQyrRN1ftfyYzhB5OcgX0an3uChcVJ3dgGtKtgNmtfrkrxdXzR3v3QuhZPdDpoJMadU6dDCnNiNc", 1);

		Runnable refreshEvent = new Runnable ()
		{
			public void run()
			{
				NetworkHandler.update();
				refresh();
				new Handler().postDelayed(this, 5000);
			}
		};
		NetworkHandler.getAllGames();
		new Handler().postDelayed(refreshEvent, 5000);
	}

	ArrayList<String> games = new ArrayList<String>();
	ArrayAdapter<String> adapter;
	private void refresh()
	{
		if(NetworkHandler.games.size() == 0)
			return;

		games.clear();

		for(int i = 0; i < NetworkHandler.games.size(); ++i)
			games.add(NetworkHandler.games.get(i).name);

		adapter.notifyDataSetChanged();
		//NetworkHandler.getOpenGames();
	}

	public void joinGame(View view)
	{
		ListView gameList = (ListView) findViewById(R.id.listView);
		long joinID = NetworkHandler.games.get(selectedGame).id;
		EditText password = (EditText) findViewById(R.id.editText2);

		NetworkHandler.join(joinID, password.getText().toString(), "");
		NetworkHandler.enter(joinID, password.getText().toString());
		Intent intent = new Intent(this, GameActivity.class);
		startActivity(intent);
	}
}
