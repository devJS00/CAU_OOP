package OOP_Proj4;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.sql.*;
import java.io.*;
import javax.swing.*;
import javax.swing.border.Border;
import org.json.simple.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;

//Save informations while enter button is clicked
class chooseActionHandler implements ActionListener 
{ 
	private static ArrayList<Assignment> assignments = new ArrayList<Assignment>();
	//private static String type_name;
	private static TextField t1;
	private static TextField t2;
	private static TextField t3;
	private static TextField t4;
	private static TextField t5;
	private static TextField t6;

	private static String sub_name;
	private static int priority;
	private static int difficulty;
	private static String memo;

	private static String title;
	private static int end_date;

	private static int exam_date;
	private static String test_name;
	public chooseActionHandler(TextField t1, TextField t2, TextField t3, TextField t4, TextField t5, TextField t6)
	{
		this.t1=t1;
		this.t2=t2;
		this.t3=t3;
		this.t4=t4;
		this.t5=t5;
		this.t6=t6;
	}
	public void actionPerformed(ActionEvent e) {
		if(!Testing.isexam)
		{
			sub_name =t1.getText().trim();
			title = t2.getText().trim();
			end_date = Integer.parseInt(t3.getText().trim());
			priority = Integer.parseInt(t4.getText().trim());
			difficulty = Integer.parseInt(t5.getText().trim());
			memo=t6.getText().trim();
			
			SaveInfo.assignmentAdd(sub_name, title, end_date, priority, difficulty, memo);
			List.assignInsert(SaveInfo.asList());
	        try {
	        if(SaveInfo.writer!=null)
	        {
	            SaveInfo.writer.flush();
	        }
		    SaveInfo.writer=new BufferedWriter(SaveInfo.file);
		    try (BufferedWriter bf = Files.newBufferedWriter(Path.of("./SaveInfos_Empty.json"),
	                StandardOpenOption.TRUNCATE_EXISTING)) {
	        } catch (IOException er) {
	            er.printStackTrace();
	        }
	        SaveInfo.writer.write(SaveInfo.ob.toJSONString());
	        if(SaveInfo.writer!=null)
	            SaveInfo.writer.flush();
	        }catch(IOException ea) {
	        	ea.printStackTrace();
	        }
		}
		else if(Testing.isexam)
		{
			sub_name =t1.getText().trim();
			test_name = t2.getText().trim();
			exam_date = Integer.parseInt(t3.getText().trim());
			priority = Integer.parseInt(t4.getText().trim());
			difficulty = Integer.parseInt(t5.getText().trim());
			memo=t6.getText().trim();
			
			SaveInfo.examAdd(sub_name, test_name, exam_date, priority, difficulty, memo);
			List.examInsert(SaveInfo.exList());
	        try {
	        if(SaveInfo.writer!=null)
	        {
	            SaveInfo.writer.flush();
	        }
		    SaveInfo.writer=new BufferedWriter(SaveInfo.file);
		    try (BufferedWriter bf = Files.newBufferedWriter(Path.of("./SaveInfos_Empty.json"),
	                StandardOpenOption.TRUNCATE_EXISTING)) {
	        } catch (IOException er) {
	            er.printStackTrace();
	        }
	        SaveInfo.writer.write(SaveInfo.ob.toJSONString());
	        if(SaveInfo.writer!=null)
	            SaveInfo.writer.flush();
	        }catch(IOException ea) {
	        	ea.printStackTrace();
	        }
		}
		t1.setText("");
		t2.setText("");
		t3.setText("");
		t4.setText("");
		t5.setText("");
		t6.setText("");
	}
}

//Create Panels by the input of assignment button or exam button
class typeHandler implements ActionListener
{	
	JLabel JL1;
	JLabel JL2;
	JLabel JL3;
	JLabel JL4;
	JLabel JL5;
	
	JButton b;
	JPanel thirdPanel;
	public typeHandler(JButton b,JPanel j)
	{
		this.b=b;
		this.thirdPanel=j;
	}
	public void actionPerformed(ActionEvent e) {
		if(b.getText().equals("Assignment"))
		{
			Testing.isexam=false;
			thirdPanel.add(Testing.createAInfoPanel());	
		}
		else if(b.getText().equals("Exam"))
		{	
			Testing.isexam=true;
			thirdPanel.add(Testing.createAInfoPanel());
		}
        Testing.tx.setVisible(true);
        Testing.tx2.setVisible(true);
        Testing.tx3.setVisible(true);
        Testing.tx4.setVisible(true);
        Testing.tx5.setVisible(true);
        Testing.tx6.setVisible(true);
	}
}

//All UIs
public class Testing {	
	public static boolean isexam;
	static int date = 1213;
	static String j1;
	static String j2;
	static String j3;
	static String j4;
	static String j5;
	static String j6;
	public static JPanel calendarPanel()
	{
        JPanel firstPanel = new JPanel();
        firstPanel.setPreferredSize(new Dimension(800, 600));
        firstPanel.setBackground(Color.white);
       
    	calendardiary cl = new calendardiary();
        firstPanel.add(cl);
        return firstPanel;
	}
	static void assignmentLabel()
	{
		j1="Sub_name";
		j2="Title";
		j3="End_date";
		j4="Importance";
		j5="Difficulty";
		j6="Memo";
	}
	static void examLabel()
	{
		j1="Sub_name";
		j2="Test_range";
		j3="Exam_date";
		j4="Importance";
		j5="Difficulty";
		j6="Memo";
	}
	static JPanel createPInfoPanel(int i, JSONObject js)//pinfopanel 틀에 집어넣을 JPanel
	{
		JPanel j = new JPanel();
        j.setPreferredSize(new Dimension(250, 150));
        j.setBackground(new Color(245,245,220));
		if(js.get("type_name").equals("assignment"))
		{
	        assignmentLabel();
			JLabel jl1 = new JLabel(j1+" : "+js.get("subject_name"));
	        jl1.setPreferredSize(new Dimension(230, 15));
	        j.add(jl1);
			JLabel jl2 = new JLabel(j2+" : "+js.get("title"));
	        jl2.setPreferredSize(new Dimension(230, 15));
	        j.add(jl2);
			JLabel jl3 = new JLabel(j3+" : "+js.get("end_date"));
			jl3.setPreferredSize(new Dimension(230, 15));
			j.add(jl3);
			JLabel jl4 = new JLabel(j4+" : "+js.get("priority"));
			jl4.setPreferredSize(new Dimension(230, 15));
			j.add(jl4);
			JLabel jl5 = new JLabel(j5+" : "+js.get("difficulty"));
			jl5.setPreferredSize(new Dimension(230, 15));
			j.add(jl5);
			JLabel jl6 = new JLabel(j6+" : "+js.get("memo"));
			jl6.setPreferredSize(new Dimension(230, 15));
			j.add(jl6);
		}
		else if(js.get("type_name").equals("exam"))
		{
	        examLabel();
			JLabel jl1 = new JLabel(j1+" : "+js.get("subject_name"));
	        jl1.setPreferredSize(new Dimension(230, 15));
	        j.add(jl1);
			JLabel jl2 = new JLabel(j2+" : "+js.get("test_range"));
	        jl2.setPreferredSize(new Dimension(230, 15));
	        j.add(jl2);
			JLabel jl3 = new JLabel(j3+" : "+js.get("exam_date"));
			jl3.setPreferredSize(new Dimension(230, 15));
			j.add(jl3);
			JLabel jl4 = new JLabel(j4+" : "+js.get("priority"));
			jl4.setPreferredSize(new Dimension(230, 15));
			j.add(jl4);
			JLabel jl5 = new JLabel(j5+" : "+js.get("difficulty"));
			jl5.setPreferredSize(new Dimension(230, 15));
			j.add(jl5);
			JLabel jl6 = new JLabel(j6+" : "+js.get("memo"));
			jl6.setPreferredSize(new Dimension(230, 15));
			j.add(jl6);
		}
		return j;
	}
	static JPanel secondPanel = new JPanel();
	public static JPanel printInfoPanel()
	{
		ArrayList<JSONObject> jslist=new ArrayList<JSONObject>();
		ArrayList<JPanel> jpanellist=new ArrayList<JPanel>();
		jslist = SaveInfo.showSched(date);

        secondPanel.setPreferredSize(new Dimension(300, 600));
        secondPanel.setBackground(Color.white);
        Component[] components=secondPanel.getComponents();
        for(Component component: components)
        	secondPanel.remove(component);
        
        JLabel label=new JLabel("Date_Info");
        label.setVerticalAlignment(JLabel.NORTH);
        label.setFont(new Font("Verdana", Font.PLAIN, 20));
        Border border = BorderFactory.createLineBorder(new Color(207, 227, 250) ,3);
        label.setBorder(border);
        secondPanel.add(label);
        
        for(int i=0 ; i<jslist.size() ; i++)
            jpanellist.add(createPInfoPanel(jslist.size(),jslist.get(i)));
        JPanel sec_1=new JPanel();
        sec_1.setPreferredSize(new Dimension(300, 150));
        sec_1.setBackground(Color.white);
        sec_1.add(new JLabel("Schedule1"));
        if(jslist.size() > 0)
        {
        	sec_1.setVisible(true);
            sec_1.add(jpanellist.get(0));
        }
        else 
        	sec_1.setVisible(false);
        
        JPanel sec_2=new JPanel();
        sec_2.setPreferredSize(new Dimension(300, 150));
        sec_2.setBackground(Color.white);
        sec_2.add(new JLabel("Schedule2"));
        if(jslist.size() > 1)
        {
        	sec_2.setVisible(true);
            sec_2.add(jpanellist.get(1));
        }
        else 
        	sec_2.setVisible(false);
        
        JPanel sec_3=new JPanel();
        sec_3.setPreferredSize(new Dimension(300, 150));
        sec_3.setBackground(Color.white);
        sec_3.add(new JLabel("Schedule3"));
        if(jslist.size() > 2)
        {
        	sec_3.setVisible(true);
            sec_3.add(jpanellist.get(2));
        }
        else 
        	sec_3.setVisible(false);
        
        secondPanel.add(sec_1);
        secondPanel.add(sec_2);
        secondPanel.add(sec_3);
        return secondPanel;
	}
	static JPanel thirdPanel = new JPanel();
	static JPanel j=new JPanel();
	static JButton chooseButton= new JButton("Enter");
	public static TextField tx = new TextField(20);
	public static TextField tx2 = new TextField(20);
	public static TextField tx3 = new TextField(20);
	public static TextField tx4 = new TextField(20);
	public static TextField tx5 = new TextField(20);
	public static TextField tx6 = new TextField(20);
	static JLabel jl1= new JLabel("Enter the Subject name");
	static JLabel jl2= new JLabel("Enter the test range");
	static JLabel jl3= new JLabel("Enter the exam date(ex)1218)");
	static JLabel jl4= new JLabel("Enter Importance(1~10)");
	static JLabel jl5= new JLabel("Enter Difficulty(1~10)");
	static JLabel jl6= new JLabel("Enter memo");
	
	public static JPanel createAInfoPanel()
	{
        Component[] components=j.getComponents();
        for(Component component: components)
        	j.remove(component);
        tx.setVisible(false);
        tx2.setVisible(false);
        tx3.setVisible(false);
        tx4.setVisible(false);
        tx5.setVisible(false);
        tx6.setVisible(false);
		tx.setText("");
		tx2.setText("");
		tx3.setText("");
		tx4.setText("");
		tx5.setText("");
		tx6.setText("");
		jl2.setPreferredSize(new Dimension(250,20));
		jl2.setHorizontalAlignment(SwingConstants.CENTER);
		jl3.setPreferredSize(new Dimension(250,20));
		jl3.setHorizontalAlignment(SwingConstants.CENTER);
        j.setPreferredSize(new Dimension(250, 400));
        j.setBackground(new Color(245,245,220));
        if(!isexam)
        {
      		jl2.setText("Enter the title");
    		jl3.setText("Enter the end date(ex)1218)");
        }
        else
        {
      		jl2.setText("Enter the test range");
    		jl3.setText("Enter the exam date(ex)1218)");
        }	
        j.add(jl1);
		j.add(tx);
		j.add(jl2);
		j.add(tx2);
		j.add(jl3);
		j.add(tx3);
		j.add(jl4);
		j.add(tx4);
		j.add(jl5);
		j.add(tx5);
		j.add(jl6);
		j.add(tx6);
		chooseButton.setBackground(new Color(155, 155, 250));
		chooseButton.setPreferredSize(new Dimension(120,30));
		j.add(chooseButton,BorderLayout.SOUTH);		
		return j;
	}
	
	static JButton assignButton = new JButton("Assignment");
	static JButton examButton = new JButton("Exam");
	
	public static JPanel addInfoPanel()
	{
        thirdPanel.setPreferredSize(new Dimension(300, 600));
        thirdPanel.setBackground(Color.white);
        Component[] components=thirdPanel.getComponents();
        for(Component component: components)
        	thirdPanel.remove(component);
        
        JLabel label=new JLabel("Enter Informations");
        label.setVerticalAlignment(JLabel.NORTH);
        label.setFont(new Font("Verdana", Font.PLAIN, 20));
        Border border = BorderFactory.createLineBorder(new Color(207, 227, 250) ,3);
        label.setBorder(border);
        thirdPanel.add(label);
        
        assignButton.setBackground(new Color(207, 227, 250));
        assignButton.setPreferredSize(new Dimension(120,30));
        examButton = new JButton("Exam");
        examButton.setBackground(new Color(207, 227, 250));
        examButton.setPreferredSize(new Dimension(120,30));
        thirdPanel.add(assignButton);
        thirdPanel.add(examButton);
        thirdPanel.add(createAInfoPanel());
        Testing.tx.setVisible(true);
        Testing.tx2.setVisible(true);
        Testing.tx3.setVisible(true);
        Testing.tx4.setVisible(true);
        Testing.tx5.setVisible(true);
        Testing.tx6.setVisible(true);
		
		assignButton.addActionListener(new typeHandler(assignButton,thirdPanel));
		examButton.addActionListener(new typeHandler(examButton,thirdPanel));
		chooseButton.addActionListener(new chooseActionHandler(tx,tx2,tx3,tx4,tx5,tx6));
        return thirdPanel;
	}
	public static void main(String[] args)
	{
	}
}
