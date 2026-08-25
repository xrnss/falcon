package club.rnss.launcher.injection;

import java.io.*;

public class Natives {
    public static boolean loaded = false;

    static {
        try {
            // We cannot load the .dll straight from the resources, so we must create a temporary file and copy it out
            File dll = File.createTempFile("falconinj-", ".dll");
            dll.deleteOnExit();

            InputStream in = Natives.class.getResourceAsStream("/FalconLauncherNatives.dll");

            try (OutputStream out = new FileOutputStream(dll)) {
                byte[] buffer = new byte[8192];
                int n;
                while ((n = in.read(buffer)) != -1) {
                    out.write(buffer, 0, n);
                }
            }

            System.load(dll.getAbsolutePath());
            loaded = true;
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public native boolean inject(int pid, byte[] dll, int port);
}