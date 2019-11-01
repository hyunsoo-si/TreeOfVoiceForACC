using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;

public class Irsensor2 : MonoBehaviour
{
    // Start is called before the first frame update
    private SerialPort serial;
    [SerializeField]
    private string arduinoPortName = "COM8";
    [SerializeField]
    private string baudRate;
    public string message;

    void Start()
    {
        serial = new SerialPort(arduinoPortName, int.Parse(baudRate), Parity.None, 8, StopBits.None);
        serial.Open();
        try
        {
            Debug.Log("Open Sream");
            Debug.Log("goooo");

            serial.Open();
            Debug.Log("goooo");
            serial.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
            serial.ReadTimeout = 10;

        }
        catch (System.Exception e)
        {
            Debug.Log("Error opening port " + e.ToString());
            // return;

        }



    }
    void OnApplicationQuit()
    {
        serial.Close();

    }
    // Update is called once per frame
    void Update()
    {
        if (serial.IsOpen)
        {
            string rData = serial.ReadLine();
            Debug.Log(rData);
        }

    }
    private void DataReceivedHandler(object sender, SerialDataReceivedEventArgs args)
    {
        SerialPort stream = (SerialPort)sender;
        string Data = stream.ReadLine();
        Debug.Log("Data Received Finish");
        Debug.Log(Data);
    }

}
