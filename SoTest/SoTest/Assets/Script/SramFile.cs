using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;

public static class SramFile
{
    private const int SRAM_SIZE = 2048;         //能存储的数据长度
    public static int m_usingLen;			    //sram使用长度
    public static bool m_isDirty;
    static byte [] m_data = null;
    static byte [] m_data_back = null;

    static string path_save;

    // Start is called before the first frame update
    public static void Init()
    { 
        Debug.Log("Sram Init!");
    
        m_data = new byte[SRAM_SIZE];
        m_data_back = new byte[SRAM_SIZE];
        m_usingLen = 100;

        m_isDirty = false;
        
        //存储路径
		string destination = Application.persistentDataPath;
		
		// //若没路径 创建
		// if (!Directory.Exists((destination)))
		// {
		// 	Directory.CreateDirectory(destination);
		// }

		//文件完整路径
		path_save = destination + "/sram";
   
        FileInfo t = new FileInfo(path_save);
        //判断文件是否存在
        if (!t.Exists)
        {
            t.CreateText();//不存在，创建
            Debug.Log("create file:"+path_save);
        }
        else
        {
            Debug.Log("open file:"+path_save);
        }

        //读取文件
        byte []temp_data = new byte[SRAM_SIZE];
        temp_data = File.ReadAllBytes(path_save);
        
        //如果原来的文件为空，m_data全部赋0.
        if(temp_data.Length==0)
        {
            for(int i = 0;i<SRAM_SIZE;i++)
            {
                temp_data[i]=0;
            }
        }
        Array.Copy(temp_data, m_data, temp_data.Length);
        Array.Copy(m_data, m_data_back, m_data.Length);
    }

    public static int Read(int begin_pos,int len)
    {    
        int ret = 0;

        //最大只保留u32的数据
        if(len>4)
        {
            len = 4;
        }
        for(int i = 0;i<len;i++)
        {
            ret += m_data[begin_pos+i]<<((len-i-1)*8);
        }

        return ret;
    }

    public static void Write()
    {
        if(m_isDirty)
        {
            //保证备份数据和使用数据一致
            Array.Copy(m_data, m_data_back, m_data.Length);
            File.WriteAllBytes(path_save, m_data);
            m_isDirty = false;
        }
    }

   public static void Test()
    {
        Init();

        int temp = Read(0,1);
        Debug.Log("befor:"+temp);

        m_data[0] += 1;
        m_isDirty = true;
        
        Write();

        temp = Read(0,1);
        Debug.Log("after:"+temp);
    }

}
