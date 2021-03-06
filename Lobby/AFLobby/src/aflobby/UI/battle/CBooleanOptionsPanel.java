/*
 * CBooleanOptionsPanel.java
 *
 * Created on 11 January 2008, 21:25
 */

package aflobby.UI.battle;

import aflobby.framework.CEvent;
import aflobby.IBattleModel;
import aflobby.LMain;
import aflobby.framework.IModule;
import javax.swing.SwingUtilities;

/**
 *
 * @author  tarendai-std
 */
public class CBooleanOptionsPanel extends javax.swing.JPanel implements IModule{
    
    int optionType;
    boolean defValue;
    String title;
    String description;
    String key;
    IBattleModel battlemodel;
    boolean host;
    boolean map;
    
    /** Creates new form CBooleanOptionsPanel
     * @param battlemodel 
     * @param key
     * @param defValue
     * @param title
     * @param description
     * @param map 
     */
    public CBooleanOptionsPanel(
            IBattleModel battlemodel, String key, boolean defValue, String title, String description, boolean map) {
        
        this.host = battlemodel.AmIHost();
        this.defValue = defValue;
        this.title = new String(title);
        this.key = new String(key);
        this.description = new String(description);
        this.battlemodel = battlemodel;
        this.map = map;
        
        battlemodel.PutOption(key, defValue);
        
        Runnable doWorkRunnable = new Runnable() {

            public void run() {
                initComponents();
            }

        };
        SwingUtilities.invokeLater(doWorkRunnable);
    }
    
    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jLabel1 = new javax.swing.JLabel();
        CheckBox = new javax.swing.JCheckBox();
        jLabel2 = new javax.swing.JLabel();
        jButton2 = new javax.swing.JButton();
        jLabel3 = new javax.swing.JLabel();

        jLabel1.setFont(jLabel1.getFont().deriveFont(jLabel1.getFont().getSize()+5f));
        jLabel1.setText(title);

        CheckBox.setSelected(defValue);
        CheckBox.setText("enabled");
        CheckBox.setEnabled(host);
        CheckBox.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                CheckBoxActionPerformed(evt);
            }
        });

        jLabel2.setText(description);

        jButton2.setText("Reset");
        jButton2.setEnabled(host);
        jButton2.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jButton2ActionPerformed(evt);
            }
        });

        jLabel3.setIcon(new javax.swing.ImageIcon(getClass().getResource("/images/UI/map.png"))); // NOI18N
        jLabel3.setDisabledIcon(new javax.swing.ImageIcon(getClass().getResource("/images/UI/optionflag.png"))); // NOI18N
        jLabel3.setEnabled(map);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(jLabel2, javax.swing.GroupLayout.DEFAULT_SIZE, 380, Short.MAX_VALUE)
                    .addGroup(javax.swing.GroupLayout.Alignment.LEADING, layout.createSequentialGroup()
                        .addComponent(CheckBox, javax.swing.GroupLayout.PREFERRED_SIZE, 147, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 142, Short.MAX_VALUE)
                        .addComponent(jButton2, javax.swing.GroupLayout.PREFERRED_SIZE, 91, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jLabel1, javax.swing.GroupLayout.DEFAULT_SIZE, 308, Short.MAX_VALUE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(jLabel3)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jLabel3, javax.swing.GroupLayout.DEFAULT_SIZE, 36, Short.MAX_VALUE)
                    .addComponent(jLabel1, javax.swing.GroupLayout.PREFERRED_SIZE, 36, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jLabel2)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(CheckBox)
                    .addComponent(jButton2))
                .addContainerGap(13, Short.MAX_VALUE))
        );
    }// </editor-fold>//GEN-END:initComponents

    private void jButton2ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jButton2ActionPerformed
        Runnable doWorkRunnable = new Runnable() {

            public void run() {
                CheckBox.setSelected(defValue);
                battlemodel.PutOption(key, CheckBox.isSelected());
            }

        };
        SwingUtilities.invokeLater(doWorkRunnable);
        
    }//GEN-LAST:event_jButton2ActionPerformed

    private void CheckBoxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_CheckBoxActionPerformed
        Runnable doWorkRunnable = new Runnable() {

            public void run() {
                battlemodel.PutOption(key, CheckBox.isSelected());
            }

        };
        SwingUtilities.invokeLater(doWorkRunnable);
    }//GEN-LAST:event_CheckBoxActionPerformed
    
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JCheckBox CheckBox;
    private javax.swing.JButton jButton2;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    // End of variables declaration//GEN-END:variables

    public void Init(LMain L) {
        throw new UnsupportedOperationException("Not supported yet.");
    }

    public void Update() {
        
    }

    public void NewEvent(CEvent e) {
        if(e.IsEvent("SETSCRIPTTAGS")){
            //
            String[] pairs = e.parameters.split("\t");
            
            for(int i = 0; i< pairs.length; i++){
                
                String[] pair = pairs[i].split("=");
                
                String k = pair[0];
                k = k.replaceAll("\\\\", "/");
                final String value = pair[1];
                
                if(k.equalsIgnoreCase(key)){
                    Runnable doWorkRunnable = new Runnable() {

                        public void run() {
                            CheckBox.setSelected(Integer.parseInt(value) == 1);
                        }

                    };
                    SwingUtilities.invokeLater(doWorkRunnable);
                    
                }
                
            }
        }
    }

    public void NewGUIEvent(CEvent e) {

    }

    public void OnRemove() {
        battlemodel = null;
    }
    
}
