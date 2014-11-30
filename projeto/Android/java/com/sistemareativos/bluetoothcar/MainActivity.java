package com.sistemareativos.bluetoothcar;

import android.app.Activity;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import java.io.IOException;
import java.io.OutputStream;
import java.util.UUID;


public class MainActivity extends Activity {
    private static final UUID BLUETOOTH_UUID =
            UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private static final int REQUEST_ENABLE_BT = 1;

    private Button m_btn_up, m_btn_down, m_btn_stop;

    private BluetoothAdapter m_bth_adapter = null;
    private BluetoothSocket m_bth_socket = null;
    private OutputStream m_out_stream = null;

    private static String s_bth_address = "20:14:04:11:10:60";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        m_btn_up = (Button) findViewById(R.id.btn_up);
        m_btn_down = (Button) findViewById(R.id.btn_down);
        m_btn_stop = (Button) findViewById(R.id.btn_stop);

        m_bth_adapter = BluetoothAdapter.getDefaultAdapter();

        m_btn_up.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SendMessage("1");
            }
        });
        m_btn_down.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SendMessage("2");
            }
        });
        m_btn_stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SendMessage("3");
            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();

        final ProgressDialog dialog = new ProgressDialog(this);
        dialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        dialog.setMessage("Conectando ao bluetooth");
        dialog.setIndeterminate(true);
        dialog.setCancelable(false);
        dialog.show();

        new Thread(new Runnable() {
            @Override
            public void run() {
                BluetoothDevice device = m_bth_adapter.getRemoteDevice(s_bth_address);

                try {
                    m_bth_socket = device.createRfcommSocketToServiceRecord(BLUETOOTH_UUID);
                } catch (IOException e) {
                    Toast toast = Toast.makeText(getBaseContext(), "Erro na criacao do socket", Toast.LENGTH_SHORT);
                    toast.show();
                    finish();
                }

                m_bth_adapter.cancelDiscovery();

                try {
                    m_bth_socket.connect();
                } catch (IOException e) {
                    try {
                        m_bth_socket.close();
                    } catch (IOException e1) {
                        Toast toast = Toast.makeText(getBaseContext(), "Erro na conexao do bluetooth", Toast.LENGTH_SHORT);
                        toast.show();
                    } finally {
                        finish();
                    }
                }

                try {
                    m_out_stream = m_bth_socket.getOutputStream();
                } catch (IOException e) {
                    Toast toast = Toast.makeText(getBaseContext(), "Erro ao pegar o Stream", Toast.LENGTH_SHORT);
                    toast.show();
                    finish();
                }

                dialog.dismiss();
            }
        }).start();
    }

    @Override
    protected void onPause() {
        super.onPause();

//        if (m_out_stream != null) {
//            try {
//                m_out_stream.flush();
//            } catch (IOException e) {
//                Toast toast = Toast.makeText(getBaseContext(), "Erro ao dar flush", Toast.LENGTH_SHORT);
//                toast.show();
//                finish();
//            }
//        }

        try {
            m_bth_socket.close();
        } catch (IOException e) {
            Toast toast = Toast.makeText(getBaseContext(), "Erro ao fechar o socket", Toast.LENGTH_SHORT);
            toast.show();
            finish();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    private void SendMessage(String message) {
        byte[] msg_buffer = message.getBytes();

        try {
            m_out_stream.write(msg_buffer);
        } catch (IOException e) {
            String msg = "In onResume() and an exception occurred during write: " + e.getMessage();
            msg = msg +  ".\n\nCheck that the SPP UUID: " + BLUETOOTH_UUID.toString() + " exists on server.\n\n";

            Toast toast = Toast.makeText(getBaseContext(), msg, Toast.LENGTH_SHORT);
            toast.show();
        }
    }
}
