using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.Xml;
using System;
using System.Runtime.InteropServices;


public class LibTest: MonoBehaviour
{
    [DllImport("libNativeCode")]
    private static extern int MyAddFunc(int _a, int _b);
    
    [DllImport("libExboard")]
    private static extern int clear_hw_sram(int _a, int _b);

    private string m_unity3dPath = string.Empty;

    public Text m_text;
    // Start is called before the first frame update
    void Start()
    {
        SramFile.Test();

        m_text.text=MyAddFunc(1,4517844).ToString();
        int ret = MyAddFunc(200, 200);
        Debug.LogFormat("--- ret:{0}", ret);

        ret = clear_hw_sram(30, 80);
        Debug.LogFormat("--- clear_hw_sram:{0}", ret);

        m_unity3dPath = Application.dataPath +"!assets/"+ "uistand";
        Debug.Log("LoadResources, m_unity3dPath:"+m_unity3dPath);

        StartCoroutine(LoadResAsync());


    }

    IEnumerator LoadResources()
    {
        
        AssetBundle assetbundle = AssetBundle.LoadFromFile(m_unity3dPath);
        
        // WWW bundle_res = WWW.LoadFromCacheOrDownload(m_unity3dPath,new Hash128(1,1,1,1));
        // yield return bundle_res;

        Debug.Log("LoadResources,assetbundle:"+assetbundle);
        if (assetbundle == null)
        {
            Debug.LogError("--Error--> Unity3d's bundle is null!");
            Debug.DebugBreak();
        }
        else
        {
            LoadFromBundle(assetbundle);
        }
        yield return null;
    }

    IEnumerator LoadResAsync()
    {
        var bundleLoadRequest = AssetBundle.LoadFromFileAsync(m_unity3dPath);
        yield return bundleLoadRequest;
        
        Debug.Log("LoadResources,assetbundle:"+bundleLoadRequest);
        
        var myLoadedAssetBundle = bundleLoadRequest.assetBundle;
        if (myLoadedAssetBundle == null)
        {
            Debug.Log("Failed to load AssetBundle!");
            yield break;
        }
        else
        {
            LoadFromBundle(myLoadedAssetBundle);
        }

        // myLoadedAssetBundle.Unload(false);
    }

    public void LoadFromBundle(AssetBundle bundle)
    {
        if (bundle == null)
        {
            Debug.LogError("--Error--> LoadFromBundle Audio bundle is null!");
            return;
        }

        // Audio config file
        //TextAsset cfgText = bundle.mainAsset as TextAsset;
        TextAsset cfgText = bundle.LoadAsset<TextAsset>("_AssetNamesConfig");
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(cfgText.text);
        Debug.Log("standui load from bundle:");
        // Parse config file
        XmlNode rootNode = doc.SelectSingleNode("doc");
        XmlNodeList assetNodes = rootNode.SelectNodes("Asset");
        foreach (XmlNode node in assetNodes)
        {
            GameObject uiObj = null;
            foreach (XmlAttribute att in node.Attributes)
            {
                if (att.Name == "name")
                {
                    uiObj = bundle.LoadAsset(att.Value) as GameObject;
                    if (uiObj == null)
                        Debug.LogError("--Error-->> LoadFromBundle : Can't load " + att.Value);
                    break;
                }
            }
            if (uiObj != null)
            {
                Debug.Log("uiObj,name:"+uiObj.name);
            }
        }
    }

    // Update is called once per frame
    void Update()
    {
        //m_text.text="hello word";
        m_text.text=MyAddFunc(1,42344).ToString();        
    }
}

// 07-19 22:24:24.935 3230-3250/com.test.cdd D/   Unity: LoadResources use pack res,assetRes filename:/data/app/com.test.cdd-1/base.apk!assets/Load/SuperChildren/Android/UIPrefabs
// 07-19 22:42:17.882 3716-3737/com.test.sotest D/Unity: LoadResources, m_unity3dPath:/data/app/com.test.sotest-1/base.apk!assets/uistand: 0