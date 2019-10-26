using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

using Random = UnityEngine.Random;

public class LEDColorGenController : MonoBehaviour
{
    // Areas of LED lights and person

    public float personDepth = 1; // 1m
    public float personWidth = 0.5f; // m

    public float innerCircleRadius = 1; // m
    public float outerCircleRadius = 2;




    // Setup events for sending LED data to m_LEDMasterController

    public int m_totalNumOfLeds = 7 + 5 + 10 + 10;

    public delegate void LEDSenderHandler(byte[] LEDArray);
    public event LEDSenderHandler m_ledSenderHandler;

    byte[] m_LEDArray;

    public SimpleBoidsTreeOfVoice m_boidComponent; // specified in the inspector

    // m_BoidsNum;
    //      BoidData[] m_boidArray;

    // Add m_ledSenderHandler.Invoke( m_LEDArray ) when m_LEDArray is ready

    // ComputeBuffer: GPU data buffer, mostly for use with compute shaders.
    // you can create & fill them from script code, and use them in compute shaders or regular shaders.


    // Declare other Component _boids; you can drag any gameobject that has that Component attached to it.
    // This will acess the Component directly rather than the gameobject itself.


    //[SerializeField]  SimpleBoidsTreeOfVoice _boids; // _boids.BoidBuffer is a ComputeBuffer
    //[SerializeField] Material _instanceMaterial;

    // [SerializeField] protected Vector3 RoomMinCorner = new Vector3(-10f, 0f, -10f);
    // [SerializeField] protected Vector3 RoomMaxCorner = new Vector3(10f, 12f, 10f);


    // 보이드의 수
    // public int BoidsNum = 256;




    // 보이드의 수
    int m_boidsNum;




    //m_neuroHeadSetController.onAverageSignalReceived += m_ledColorGenController.UpdateLEDResponseParameter;
    //m_irSensorMasterController.onAverageSignalReceived += m_ledColorGenController.UpdateColorBrightnessParameter;

    void Start()
    {

        if (m_boidComponent == null)
        {
            Debug.LogError("_boids component is not set in the inspector");
            Application.Quit();

        }

        m_boidsNum = (int)m_boidComponent.m_BoidsNum;

        m_LEDArray = new byte[m_totalNumOfLeds * 3];


    }


    public void UpdateLEDResponseParameter(double[] electrodeData) {
    }

    public 
        void UpdateColorBrightnessParameter(int[] irDistances) {
    }


    void Update()
    {
      
    
    //public static float/iny Range(float min, float max);

        for (int i = 0; i < m_totalNumOfLeds; i++)
        {
            int k = Random.Range(0, m_boidsNum);

            m_LEDArray[i * 3] = (byte) (255 * m_boidComponent.m_boidArray[k].Color[0] ); // Vector4 Color
            m_LEDArray[i * 3 +1] = (byte) ( 255 * m_boidComponent.m_boidArray[k].Color[1] );
            m_LEDArray[i * 3 +2] = (byte) (255 *  m_boidComponent.m_boidArray[k].Color[2] );


        }

        m_ledSenderHandler.Invoke( m_LEDArray) ;

 
        }



} //  LEDColorGenController class
