/*
 * JPrivate.java
 *
 * Created on 27 May 2006, 21:57
 */

package jlobby;

import java.util.HashMap;
import javax.swing.Action;
import javax.swing.text.DefaultEditorKit;
import javax.swing.text.JTextComponent;

/**
 *
 * @author  AF
 */

public class JPrivate extends javax.swing.JFrame {
    public LMain LM;
    public JPlayer player;
    public boolean opened = false;
    public java.awt.Point origin= new java.awt.Point();
    public int lastsent = 0; // 1 is me, 2 is other person Used for message accumulation

    private HashMap actions;
    /** Creates new form JPrivate */
    public JPrivate() {
        initComponents();
        
    }
    public void Init(LMain L, JPlayer p){
        //
        LM = L;
        player = p;
        jLabel1.setText("PM :"+p.name + " :: " + p.connection.server);
        this.setTitle("PM :"+p.name + " :: " + p.connection.server);
    }
    
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    // <editor-fold defaultstate="collapsed" desc=" Generated Code ">//GEN-BEGIN:initComponents
    private void initComponents() {
        jPopupMenu1 = new javax.swing.JPopupMenu();
        jMenuItem1 = new javax.swing.JMenuItem();
        jMenuItem2 = new javax.swing.JMenuItem();
        jMenuItem3 = new javax.swing.JMenuItem();
        jSeparator2 = new javax.swing.JSeparator();
        jMenuItem4 = new javax.swing.JMenuItem();
        jPanel1 = new javax.swing.JPanel();
        jLabel1 = new javax.swing.JLabel();
        jSplitPane1 = new javax.swing.JSplitPane();
        jPanel2 = new javax.swing.JPanel();
        jToolBar1 = new javax.swing.JToolBar();
        jButton2 = new javax.swing.JButton();
        jToggleButton1 = new javax.swing.JToggleButton();
        jButton4 = new javax.swing.JButton();
        jButton5 = new javax.swing.JButton();
        jScrollPane1 = new javax.swing.JScrollPane();
        MsgText = new javax.swing.JTextArea();
        jButton1 = new javax.swing.JButton();
        jScrollPane2 = new javax.swing.JScrollPane();
        jTextArea1 = new javax.swing.JTextArea();
        jButton3 = new javax.swing.JButton();

        createActionTable(jTextArea1);
        jMenuItem1.setAction(getActionByName(DefaultEditorKit.cutAction));
        jMenuItem1.setMnemonic('t');
        jMenuItem1.setText("Cut");
        jMenuItem1.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jMenuItem1MousePressed(evt);
            }
        });

        jPopupMenu1.add(jMenuItem1);

        jMenuItem2.setAction(getActionByName(DefaultEditorKit.copyAction));
        jMenuItem2.setMnemonic('c');
        jMenuItem2.setText("Copy");
        jPopupMenu1.add(jMenuItem2);

        jMenuItem3.setAction(getActionByName(DefaultEditorKit.pasteAction));
        jMenuItem3.setMnemonic('p');
        jMenuItem3.setText("Paste");
        jPopupMenu1.add(jMenuItem3);

        jPopupMenu1.add(jSeparator2);

        jMenuItem4.setMnemonic('A');
        jMenuItem4.setText("Select All");
        jPopupMenu1.add(jMenuItem4);

        setResizable(false);
        setUndecorated(true);
        addMouseMotionListener(new java.awt.event.MouseMotionAdapter() {
            public void mouseDragged(java.awt.event.MouseEvent evt) {
                formMouseDragged(evt);
            }
        });
        addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                formMousePressed(evt);
            }
        });

        jPanel1.setBorder(javax.swing.BorderFactory.createEtchedBorder());
        jPanel1.addMouseMotionListener(new java.awt.event.MouseMotionAdapter() {
            public void mouseDragged(java.awt.event.MouseEvent evt) {
                jPanel1MouseDragged(evt);
            }
        });
        jPanel1.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jPanel1MousePressed(evt);
            }
        });

        jLabel1.setFont(new java.awt.Font("Arial", 0, 11));
        jLabel1.setText("User :: server");

        jSplitPane1.setBorder(null);
        jSplitPane1.setDividerLocation(200);
        jSplitPane1.setDividerSize(3);
        jSplitPane1.setOrientation(javax.swing.JSplitPane.VERTICAL_SPLIT);
        jToolBar1.setBorder(null);
        jToolBar1.setFloatable(false);
        jToolBar1.setBorderPainted(false);
        jButton2.setFont(new java.awt.Font("Arial", 0, 11));
        jButton2.setText("Spellcheck");
        jButton2.setBorderPainted(false);
        jButton2.setOpaque(false);
        jToolBar1.add(jButton2);

        jToggleButton1.setFont(new java.awt.Font("Arial", 0, 11));
        jToggleButton1.setSelected(true);
        jToggleButton1.setText("Accumulate messages");
        jToggleButton1.setBorderPainted(false);
        jToolBar1.add(jToggleButton1);

        jButton4.setFont(new java.awt.Font("Arial", 0, 11));
        jButton4.setText("Undo");
        jButton4.setBorderPainted(false);
        jButton4.setOpaque(false);
        jToolBar1.add(jButton4);

        jButton5.setFont(new java.awt.Font("Arial", 0, 11));
        jButton5.setText("Redo");
        jButton5.setBorderPainted(false);
        jButton5.setOpaque(false);
        jToolBar1.add(jButton5);

        MsgText.setColumns(20);
        MsgText.setComponentPopupMenu(jPopupMenu1);
        MsgText.setFont(new java.awt.Font("Arial", 0, 12));
        MsgText.setLineWrap(true);
        MsgText.setRows(2);
        MsgText.setNextFocusableComponent(jButton1);
        jScrollPane1.setViewportView(MsgText);

        jButton1.setText("Send");
        jButton1.setDoubleBuffered(true);
        jButton1.setNextFocusableComponent(MsgText);
        jButton1.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jButton1MousePressed(evt);
            }
        });

        org.jdesktop.layout.GroupLayout jPanel2Layout = new org.jdesktop.layout.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, jPanel2Layout.createSequentialGroup()
                .add(jScrollPane1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 314, Short.MAX_VALUE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jButton1)
                .add(3, 3, 3))
            .add(jToolBar1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 382, Short.MAX_VALUE)
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel2Layout.createSequentialGroup()
                .add(jToolBar1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 23, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .add(0, 0, 0)
                .add(jPanel2Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(jButton1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 62, Short.MAX_VALUE)
                    .add(jScrollPane1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 62, Short.MAX_VALUE)))
        );
        jSplitPane1.setBottomComponent(jPanel2);

        jScrollPane2.setDoubleBuffered(true);
        jScrollPane2.setMinimumSize(new java.awt.Dimension(23, 100));
        jTextArea1.setColumns(20);
        jTextArea1.setComponentPopupMenu(jPopupMenu1);
        jTextArea1.setEditable(false);
        jTextArea1.setFont(new java.awt.Font("Arial", 0, 12));
        jTextArea1.setLineWrap(true);
        jTextArea1.setRows(5);
        jTextArea1.setWrapStyleWord(true);
        jTextArea1.setMinimumSize(new java.awt.Dimension(160, 100));
        jScrollPane2.setViewportView(jTextArea1);

        jSplitPane1.setLeftComponent(jScrollPane2);

        jButton3.setText("X");
        jButton3.setBorderPainted(false);
        jButton3.setMargin(new java.awt.Insets(0, 0, 0, 0));
        jButton3.setOpaque(false);
        jButton3.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                jButton3MousePressed(evt);
            }
        });

        org.jdesktop.layout.GroupLayout jPanel1Layout = new org.jdesktop.layout.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, jPanel1Layout.createSequentialGroup()
                .add(jLabel1)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED, 302, Short.MAX_VALUE)
                .add(jButton3))
            .add(org.jdesktop.layout.GroupLayout.TRAILING, jSplitPane1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 382, Short.MAX_VALUE)
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel1Layout.createSequentialGroup()
                .add(jPanel1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(jButton3)
                    .add(jLabel1))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED, 0, Short.MAX_VALUE)
                .add(jSplitPane1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 288, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
        );

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );
        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void jPanel1MousePressed(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jPanel1MousePressed
        origin.x = evt.getX();
        origin.y = evt.getY();
    }//GEN-LAST:event_jPanel1MousePressed

    private void jPanel1MouseDragged(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jPanel1MouseDragged
        java.awt.Point p = getLocation();
        setLocation(
        p.x + evt.getX() - origin.x, 
        p.y + evt.getY() - origin.y);
    }//GEN-LAST:event_jPanel1MouseDragged

    private void jMenuItem1MousePressed(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jMenuItem1MousePressed
    }//GEN-LAST:event_jMenuItem1MousePressed

    private void formMouseDragged(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_formMouseDragged
    java.awt.Point p = getLocation();
        setLocation(
        p.x + evt.getX() - origin.x, 
        p.y + evt.getY() - origin.y);
    }//GEN-LAST:event_formMouseDragged

    private void formMousePressed(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_formMousePressed
        origin.x = evt.getX();
        origin.y = evt.getY();
    }//GEN-LAST:event_formMousePressed

    private void jButton3MousePressed(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton3MousePressed
// TODO add your handling code here:
        Hide();
    }//GEN-LAST:event_jButton3MousePressed

    private void jButton1MousePressed(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_jButton1MousePressed
// TODO add your handling code here:
        if(LM.connection.IsConnected()==true){
            //
            String lines[] = MsgText.getText().split("\n");
            for(int n = 0; n <lines.length; n++){
                String command[] = lines[n].split(" ");
                if(command[0].equalsIgnoreCase("/me")){
                    LM.connection.SendLine("SAYPRIVEX " + player.name + " " + Misc.makeSentence(command,1));
                }else{
                    LM.connection.SendLine("SAYPRIVATE " + player.name + " " + lines[n]);
                }
            }
            if(jToggleButton1.isSelected()==true){
                for(int n = 0; n <lines.length; n++){
                    String command[] = lines[n].split(" ");
                    if(command[0].equalsIgnoreCase("/me")){
                        Add(player.connection.username + " " + Misc.makeSentence(command,1));
                    }else{
                        if(lastsent==0){
                            Add(player.connection.username + " Says:\n   " + lines[n]);
                            lastsent=1;
                        }else if(lastsent==2){
                            Add(player.connection.username + " Says:\n   " + lines[n]);
                            lastsent=1;
                        }else{
                            Add("   " + lines[n]);
                        }
                    }
                }
            }else{
                for(int n = 0; n <lines.length; n++){
                    Add(player.connection.username + " Says:\n   " + lines[n]);
                }
                lastsent=1;
            }
            //lastsent=1;
            MsgText.setText("");
        }
    }//GEN-LAST:event_jButton1MousePressed
    
    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new JPrivate().setVisible(false);
            }
        });
    }
    private void createActionTable(JTextComponent textComponent) {
        actions = new HashMap();
        Action[] actionsArray = textComponent.getActions();
        for (int i = 0; i < actionsArray.length; i++) {
            Action a = actionsArray[i];
            actions.put(a.getValue(Action.NAME), a);
        }
    }
    private Action getActionByName(String name) {
        return (Action)(actions.get(name));
    }
    public void Add(String s){
        //
        jTextArea1.append(s + "\n");
        jTextArea1.setCaretPosition(jTextArea1.getText().length());
    }
    public void Update(){
        //
    }
    public void Open(){
        setVisible(true);
    }
    public void Hide(){
        setVisible(false);
    }
    public void NewEvent(JEvent e){
        if(e.data[0].equals("SAIDPRIVATE")){
            if(e.data[1].equals(player.name)){
                Open();
                if(jToggleButton1.isSelected()==true){
                    if(lastsent==0){
                        Add(player.name + " Says:\n   " + Misc.makeSentence(e.data,2));
                        lastsent=2;
                    }else if(lastsent==1){
                        Add(player.name + " Says:\n   " +Misc.makeSentence(e.data,2));
                        lastsent=2;
                    }else{
                        Add("   " + Misc.makeSentence(e.data,2));
                        lastsent=2;
                    }
                }else{
                    Add(player.name + " Says:\n   " +Misc.makeSentence(e.data,2));
                    lastsent=2;
                }
            }
        }else if(e.data[0].equals("SAIDPRIVEX")){
            if(e.data[1].equals(player.name)){
                Open();
                Add(player.name + " " +Misc.makeSentence(e.data,2));
            }
        }
    }
    public void NewGUIEvent(JEvent e){
        if(e.data[0].equals("OPENPRIV")){
            if(e.data[1] == null){
                return;
            }
            if(e.data[1].equalsIgnoreCase(player.name)){
                Open();
            }
        }
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JTextArea MsgText;
    private javax.swing.JButton jButton1;
    private javax.swing.JButton jButton2;
    private javax.swing.JButton jButton3;
    private javax.swing.JButton jButton4;
    private javax.swing.JButton jButton5;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JMenuItem jMenuItem1;
    private javax.swing.JMenuItem jMenuItem2;
    private javax.swing.JMenuItem jMenuItem3;
    private javax.swing.JMenuItem jMenuItem4;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPopupMenu jPopupMenu1;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JSeparator jSeparator2;
    private javax.swing.JSplitPane jSplitPane1;
    private javax.swing.JTextArea jTextArea1;
    private javax.swing.JToggleButton jToggleButton1;
    private javax.swing.JToolBar jToolBar1;
    // End of variables declaration//GEN-END:variables
    
}
