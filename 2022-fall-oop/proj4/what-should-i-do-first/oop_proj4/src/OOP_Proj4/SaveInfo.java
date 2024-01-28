package OOP_Proj4;

import java.io.FileWriter;
import java.io.FileReader;
import java.io.IOException;
import org.json.simple.JSONObject;
import org.json.simple.JSONArray;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

import java.io.File;
import java.io.BufferedWriter;
import java.io.BufferedReader;
import java.util.ArrayList;
import java.util.Iterator;
import java.io.InputStream;
import java.util.Scanner;
import java.time.LocalDate;
import java.time.Period;

public class SaveInfo {

	private static calendardiary cl = new calendardiary();

	private static Assignment as;
	private static ArrayList<Assignment> asResult = new ArrayList<>();

	private static Exam ex;
	private static ArrayList<Exam> exResult = new ArrayList<>();

    private static SortingFunction sf;

    private static List ls;
	
	public static JSONObject ob;
	public static int scheduleNum;
	private static String assignStr="assignment";
	private static String examStr="exam";
	public static FileWriter file;
	public static BufferedWriter writer;
	
	public static void main(String[] args) throws IOException, ParseException
	{	 
		file = new FileWriter("./SaveInfos_Empty.json",true);
        String brack= "{}";
       
        BufferedReader r= new BufferedReader(new FileReader("./SaveInfos_Empty.json"));
        if(r.readLine()==null)
        {
        	writer=new BufferedWriter(file);
            writer.write(brack);
	        if(SaveInfo.writer!=null)
	        {
	            SaveInfo.writer.flush();
	        }
        }

        ob = (JSONObject)new JSONParser().parse(new FileReader("./SaveInfos_Empty.json"));        
    	
        System.out.println(ob.toString());
        JSONObject js = (JSONObject) ob;
        
        scheduleNum=ob.keySet().size();
        System.out.println(ob.keySet().size());
        
		for(int i=0;i<scheduleNum;i++)
		{
			js = (JSONObject)SaveInfo.ob.get("Sched_"+Integer.toString(i+1));
			String type_name = (String) js.get("type_name");
			if(type_name.equals(assignStr))
			{
				String sub_name=(String)js.get("subject_name");
				String title=(String)js.get("title");
				int enddate = ((Long) js.get("end_date")).intValue();
				int priority = ((Long) js.get("priority")).intValue();
				int difficulty = ((Long) js.get("difficulty")).intValue();
				String memo=(String)js.get("memo");
				Assignment newAs = new Assignment(sub_name,title, enddate ,priority,difficulty,memo);
				asResult.add(newAs);
			}
			else if(type_name.equals(examStr))
			{	
				String sub_name=(String)js.get("subject_name");
				String test_range=(String)js.get("test_range");
				int examdate = ((Long) js.get("exam_date")).intValue();
				int priority = ((Long) js.get("priority")).intValue();
				int difficulty = ((Long) js.get("difficulty")).intValue();
				String memo=(String)js.get("memo");
				Exam newEx = new Exam(sub_name,test_range,examdate,priority,difficulty,memo);
				exResult.add(newEx);
			}
		}

		
		new List(asResult,exResult);
	}
	
	//getter
	public static ArrayList<Assignment> asList()
	{
		return asResult;
	}
	public static ArrayList<Exam> exList()
	{
		return exResult;
	}
	
	//add assingment informations to JSON File
    public static void assignmentAdd(String sub_name, String title, int end_date, int priority, int difficulty, String memo)
    {
        JSONObject inner =new JSONObject();
        
        inner.put("type_name", "assignment");
        inner.put("subject_name", sub_name); 
        inner.put("title", title); 
        inner.put("end_date", end_date);
        inner.put("priority", priority);
        inner.put("difficulty", difficulty);
        inner.put("memo", memo);      
        scheduleNum++;      
        ob.put("Sched_"+Integer.toString(scheduleNum), inner);
        Assignment newAs = new Assignment(sub_name,title, end_date ,priority,difficulty,memo);
        asResult.add(newAs);
    }
    
    //add exam informations to JSON File
    public static void examAdd(String sub_name, String test_name, int exam_date, int priority, int difficulty, String memo)
    {
        JSONObject inner =new JSONObject();
        
        inner.put("type_name", "exam");
        inner.put("subject_name", sub_name); 
        inner.put("test_range", test_name); 
        inner.put("exam_date", exam_date);
        inner.put("priority", priority);
        inner.put("difficulty", difficulty);
        inner.put("memo", memo);      
        scheduleNum++;      
        ob.put("Sched_"+Integer.toString(scheduleNum), inner);
        Exam newEx = new Exam(sub_name,test_name, exam_date ,priority,difficulty,memo);
        exResult.add(newEx);
    }
    
    //Get specific Date's Informations
    public static ArrayList<JSONObject> showSched(int cDate)
    {
    	JSONObject js;
    	ArrayList<JSONObject> jslist=new ArrayList<JSONObject>();
		for(int i=0;i<SaveInfo.scheduleNum;i++)
		{
			js = (JSONObject)SaveInfo.ob.get("Sched_"+Integer.toString(i+1));
			String type_name = (String) js.get("type_name");
			if(type_name.equals("assignment"))
			{
				int end_date = 0;
				if(js.get("end_date").getClass().getSimpleName().equals("Long"))
				{
					end_date = ((Long) js.get("end_date")).intValue();
					if(cDate==end_date)
						jslist.add(js);
				}
				else if(js.get("end_date").getClass().getSimpleName().equals("Integer"))
				{
					end_date = (int) js.get("end_date");					
					if(cDate==end_date)
						jslist.add(js);
				}
			}
			else if(type_name.equals("exam"))
			{
				int exam_date = 0;
				if(js.get("exam_date").getClass().getSimpleName().equals("Long"))
				    exam_date = ((Long) js.get("exam_date")).intValue();
				else if(js.get("exam_date").getClass().getSimpleName().equals("Integer"))
					exam_date=(int) js.get("exam_date");
				
				if(cDate==exam_date)
					jslist.add(js);
			}
		}
		return jslist;
    }
}

