package com.example.administrator.facelandmark_test;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.widget.ImageView;
import android.widget.Button;
import android.content.Intent;
import android.net.Uri;

import java.io.FileNotFoundException;


public class MainActivity extends AppCompatActivity implements View.OnClickListener{
    private Button button ;
    private Button button1 ;
    private ImageView picture;
    private Uri imageUri;

    public static File tempFile;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private void copyAssetsFile(String name, File dir) throws IOException {
        if (!dir.exists()) {
            dir.mkdirs();
        }
        File file = new File(dir, name);
        if (!file.exists()){
            InputStream is = getAssets().open(name);
            FileOutputStream fos = new FileOutputStream(file);
            int len;
            byte[] buffer = new byte[2048];
            while ((len = is.read(buffer)) != -1)
                fos.write(buffer, 0, len);
            fos.close();
            is.close();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        button = findViewById(R.id.button);
        button1 = findViewById(R.id.button1);
        picture = findViewById(R.id.image);

        try {
            File dir = new File("/sdcard/facelandmark");
            copyAssetsFile("haarcascade_frontalface_alt.xml", dir);
            copyAssetsFile("shape_predictor_68_face_landmarks.dat", dir);
        } catch (IOException e) {
            e.printStackTrace();
        }

        button.setOnClickListener(this);
        button1.setOnClickListener(this);
    }

    public void onClick(View view){
        if(view.getId()==R.id.button){
            openGallery();
        }
        if(view.getId()==R.id.button1){
            ImageView tv = findViewById(R.id.image);
            tv.setImageBitmap(function1());
        }
    }

    public Bitmap  function1(){
        Bitmap bitmap =((BitmapDrawable)picture.getDrawable()).getBitmap();
        findface(bitmap);

        return bitmap;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (resultCode == RESULT_OK) {
            try {
                if(data != null) {
                    Uri uri = data.getData();
                    imageUri = uri;
                    }
                    Bitmap bitmap = BitmapFactory.decodeStream(getContentResolver().openInputStream(imageUri));
                picture.setImageBitmap(bitmap);
                    } catch (FileNotFoundException e) {
                        e.printStackTrace();
                    } }

    }

    public void openGallery() {
        Intent intent = new Intent(Intent.ACTION_PICK);
        intent.setType("image/*");
        startActivityForResult(intent, 1);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native void findface(Bitmap bitmap);
}
