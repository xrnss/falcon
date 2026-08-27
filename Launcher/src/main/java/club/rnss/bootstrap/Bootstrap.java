package club.rnss.bootstrap;

public class Bootstrap {
    public static void initiate() {
        // Bootstrap method; your entry point into your classes goes here...
        // e.g. MyClassAlsoInBootstrapJar.EntryPointMethod();
    }

    public native static byte[] getClassBytes();
    public native static boolean setClassBytes(byte[] bytes);
}