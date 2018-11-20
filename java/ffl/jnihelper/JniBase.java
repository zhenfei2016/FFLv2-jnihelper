package ffl.jnihelper;

/**
 * Created by zhufeifei on 2018/11/13.
 */

public class JniBase {
    public JniBase(){
    }
    //
    //
    //
    public static  void JniSetup(String pakcetname,Object thiz,Object weakparam){
        native_setup(pakcetname,thiz,weakparam);
    }
    public static  void JniRelease(String pakcetname,Object thiz){
        native_release(pakcetname,thiz);
    }
    //
    //  对应的native层class结构指针
    //
    private native static void native_setup(String pakcetname,Object thiz,Object weakparam);
    private native static void native_release(String pakcetname,Object thiz);

    //保存这个对象的native层的实例
    // native层使用的
    private long mNativeInstancePtr;
}

