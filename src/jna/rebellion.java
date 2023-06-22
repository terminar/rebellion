package org.rebellion;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;
import com.sun.jna.Callback;
import com.sun.jna.Pointer;

public class rebellion {

    public interface RebellionDLL extends Library {

        public static interface rebellion_message_type {
            public static final int REBELLION_MT_REQ = 0;
            public static final int REBELLION_MT_RES = 1;
            public static final int REBELLION_MT_EV = 2;
            public static final int REBELLION_MT = 3;
        }

        public static interface rebellion_message_format {
            public static final int REBELLION_MF_JSON = 0;
            public static final int REBELLION_MF = 1;
        }
        
        char []rebellion_message_type_str(rebellion_message_type mt);
        char []rebellion_message_format_str(rebellion_message_format mf);
        
        //rebellion callback
        public static interface rebellion_rpc_callback extends Callback {
                public int invoke(
                    int mf,
                    int mt,
                    Pointer data,
                    int len
                );
        }

        public int rebellion(rebellion_rpc_callback cb);
        public int rebellion_rpc(
                    int mf, 
                    int mt, 
                    Pointer data, 
                    int len
                );
        
        public int rebellion_loop(int looptime);
        
    }

    public static void main(String[] args) {

        RebellionDLL INSTANCE = (RebellionDLL) Native.loadLibrary("librebellion.dylib", RebellionDLL.class);

        RebellionDLL.rebellion_rpc_callback rebellion_rpc_cb = new RebellionDLL.rebellion_rpc_callback() {            
            public int invoke(
                    int mf,
                    int mt,
                    Pointer data,
                    int len
                ) {
                System.out.println("");
                System.out.println("Rebellion rpc callback was raised");
                System.out.println("");
                return 0;
            };
        };
        
        RebellionDLL.rebellion_rpc_callback old_handler = INSTANCE.signal(Reb.SIGUSR1, fn);
        lib.raise(CLibrary.SIGUSR1);
    
        //INSTANCE.rebellion(rebellion_rpc_cb);
        INSTANCE.rebellion_loop(0);
    }
}
