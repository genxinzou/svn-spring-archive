/*
 * CEvent.java
 *
 * Created on 27 May 2006, 21:20
 *
 * To change this template, choose Tools | Template Manager
 * and open the template in the editor.
 */

package aflobby.framework;

/**
 *
 * @author AF
 */

public class CEvent {
    public final static String DISCONNECT = "DISCONNECT";
    public final static String FAILEDCONNECTION = "FAILEDCONNECTION";
    
    public final static String CONNECTED = "connected";
    public final static String DISCONNECTED = "DISCONNECTED";
    public final static String DISCONNECTUNKNOWNHOST = "UNKNOWN_HOST";
    
    public final static String LOGOUT = "LOGOUT";
    public final static String LOGGEDOUT = "LOGGEDOUT";
    public final static String LOGINPROGRESS = "PROGRESS";
    
    public final static String OPENPRIVATEMSG = "OPENPRIV";
    
    public final static String TOGGLERAWTRAFFIC ="TOGGLETRAFFIC";
    
    public final static String TIMER ="TIMER";
    
    
    public final static String NEWUSRADDED = "NEWUSRADDED";
    
    
    
    public final static String CHANNELCLOSED ="channelclose";
    public final static String CHANNELREAD ="channeltabread";
    public final static String CHANNELUNREAD ="channeltabunread";
    
    public final static String LOGONSCRIPTCHANGE ="logonscriptchange";
    
    public final static String CONTENTREFRESH = "contentrefresh";
    
    public final static String EXITEDBATTLE = "exitedbattle";
    public final static String ADDEDBATTLE = "addedbattle";
    
    public final static String BATTLEINFO_CHANGED ="battleinfo_changed";
    
    
    //public final static String HIDERAWTRAFFIC ="HIDETRAFFIC";
    
    public String[] data;
    public String parameters=""; // Misc.makesentence(data,1);
    public Object a;
    /**
     * Creates a new instance of CEvent
     *
     * @param s 
     */
    public CEvent (final String s) {
        data = s.split (" ");
        
        if(data.length >1){
            parameters = s.substring(data[0].length()+1);
        }
    }
    
    public boolean IsEvent (String s){
        if(data == null){
            return false;
        }
        return s.equalsIgnoreCase (data[0]);
    }
    
}
