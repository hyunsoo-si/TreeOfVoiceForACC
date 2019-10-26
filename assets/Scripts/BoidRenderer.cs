using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class BoidRenderer : MonoBehaviour
{

    // ComputeBuffer: GPU data buffer, mostly for use with compute shaders.
    // you can create & fill them from script code, and use them in compute shaders or regular shaders.


     // Declare other Component _boids; you can drag any gameobject that has that Component attached to it.
     // This will acess the Component directly rather than the gameobject itself.


    [SerializeField] private SimpleBoids _boids; // _boids.BoidBuffer is a ComputeBuffer

    // [SerializeField] protected Vector3 RoomMinCorner = new Vector3(-10f, 0f, -10f);
    // [SerializeField] protected Vector3 RoomMaxCorner = new Vector3(10f, 12f, 10f);


    // 보이드의 수
    // public int BoidsNum = 256;


     protected Vector3  GroundMinCorner;
    protected Vector3  GroundMaxCorner;

    protected Vector3  CeilingMinCorner;
    protected Vector3  CeilingMaxCorner;

    private  float _scale;
    
    // 보이드의 수
    private int BoidsNum;

    // GPU Instancing
    // Graphics.DrawMeshInstancedIndirect
    // 인스턴스화 된 쉐이더를 사용하여 특정 시간동안 동일한 메시를 그릴 경우 사용
     private Mesh _instanceMeshCircle;

    [SerializeField] private Mesh _instanceMeshSphere;
    [SerializeField] private Mesh _instanceMeshHuman1;
    [SerializeField] private Mesh _instanceMeshHuman2;
    [SerializeField] private Mesh _instanceMeshHuman3;
    [SerializeField] private Mesh _instanceMeshHuman4;


    private int _meshNo; // set from SimpleBoids

    private Mesh _instanceMesh;

    [SerializeField] private Material _instanceMaterial;
    //private ComputeBuffer colorBuffer;

    //     ArgsOffset      인스턴스 당 인덱스 수    (Index count per instance)
    //                     인스턴스 수              (Instance count)
    //                     시작 인덱스 위치         (Start index location)
    //                     기본 정점 위치           (Base vertex location)
    //                     시작 인스턴스 위치       (Start instance location)
    private ComputeBuffer _argsBuffer;
    private readonly uint[] _args = new uint[5] { 0, 0, 0, 0, 0 };

    uint numIndices;
    Vector3[] vertices3D;

    int[] indices;
    


    // //Display.Length will always be 1 in the Editor. We dont have an implementation of display detection in editor as it works different. We are using Editor windows, not actual displays.
    // //You can still use multiple display in the editor you just dont need to activate the displays like you do in a release.

    // //https://stackoverflow.com/questions/43066541/unity-multiple-displays-not-working

    // void InitializeCameras()
    // {


    //     // multiple camera tutorial: http://blog.theknightsofunity.com/using-multiple-unity-cameras-why-this-may-be-important/
    //     //Did you notice that the default Unity 5 scene camera clears buffers to Skybox?
    //     ////This is Main Camera in the scene
    //     //Camera m_MainCamera;
    //     //This is the second Camera and is assigned in inspector
    //     // public Camera m_CameraTwo;

    //     // Finding multiple cameras: https://answers.unity.com/questions/15801/finding-cameras.html
    //     //(1) In your code use the following sysntax:
    //     //Camera myCamera = GameObject.FindWithTag("myCamera").GetComponent <> Camera > ();
    //     //
    //     // (2): 1) Simply drag a reference of the desired camera to a variable of type Camera in your script.

    //     //2) use Camera.main if the camera you want is the only active one right now.

    //     //3) If you have multiple cameras named uniquely, 
    //     //check foreach (Camera c in Camera.allCameras) and  c.gameObject.name == "DesiredCamera" 
    //     //then that is the camera you want.
    //     //.ner, far, fieldOfView
    //     // //Start the Camera field of view at 60
    //     //m_FieldOfView = 60.0f; This is the vertical field of view; 
    //     // .aspect = width / height
    //     //.name


    //     // Script inherits from MonoBehavior => Behavior => Component => object
    //     // Camera component inherits from Behavior => Component => object
    //     // Behaviours are Components that can be enabled or disabled.
    //     // For example, Rigidbody cannot be enabled/disabled. This is why it inherits from the Component class instead of Behaviour.
    //     // MonoBehaviour is the base class from which every Unity script derives.
    //     //MonoBehaviour:
    //     //The most important thing to note about MonoBehaviour is that you need it when you have to use corutines, Invoking,
    //     //or any Unity callback functions such as physics OnCollisionEnter function, Start, OnEnable, OnDisable, etc.
    //     //MonoBehaviour inherits from Behaviour so that your scripts can be enabled / disabled.
    //     //Note that Behaviour and Component are used by Unity for internal stuff. You should not try to inherit your script from these.


    //  //   Debug.Log("display number = " + Display.displays.Length);


    //    //   for (int i = 0; i < Display.displays.Length; i++)
    //         // scan the active cameras:
    //       foreach (Camera c in Camera.allCameras)
    //     {
    //         //Debug.Log("game name=" + c.gameObject.name);
    //         //Debug.Log("name=" + c.name);
    //         //Debug.Log("vertical field of view ="); Debug.Log(c.fieldOfView);
    //         //Debug.Log("aspect (=w/h) = "); Debug.Log(c.aspect);

    //      //   Camera c = myCams[i];

    //        // if (c.name == "MainCamera") return;


    //         if (c.name == "CameraToGroundLeft")
    //         {
    //             Debug.Log(c.name);

    //             Debug.Log("camera pos=");
    //             Debug.Log(c.transform.position);


    //             Vector3 targetPos = new Vector3( GroundMinCorner.x / 2f, 0f, 0f); 
    //             Debug.Log("camera targetPos=");
    //             Debug.Log(targetPos);


    //             Debug.Log("computed  field of view:");  
    //             Vector3 vecToTarget = targetPos - c.transform.position;

    //             float heightOnAxis = ( targetPos - new Vector3(0f,0f,GroundMinCorner.z) ).magnitude;  

    //             float fieldOfView = 2.0f * Mathf.Rad2Deg *
    //                              Mathf.Atan( heightOnAxis  / vecToTarget.magnitude);
    //             Debug.Log(fieldOfView);

    //             Debug.Log("computed aspect1 (w/h):");
    //             float aspect =   Mathf.Abs( GroundMinCorner.x )   / (2f * heightOnAxis) ;
    //             Debug.Log(aspect);

    //             Debug.Log("computed aspect2 (w/h):");
    //              aspect = Mathf.Abs(GroundMinCorner.x) / (2 * Mathf.Abs(GroundMinCorner.z) );
    //             Debug.Log(aspect);


    //             c.fieldOfView = fieldOfView;
    //             c.aspect = aspect;

    //         }

    //         if (c.name == "CameraToGroundRight")
    //         {
    //             Debug.Log(c.name);

    //             Debug.Log("camera pos=");
    //             Debug.Log(c.transform.position);


    //             Vector3 targetPos =  new Vector3(GroundMaxCorner.x/2f, 0f, 0f) ;
    //             Debug.Log("camera targetPos=");
    //             Debug.Log(targetPos);


    //             Debug.Log("computed  field of view:");
    //             Vector3 vecToTarget = targetPos - c.transform.position;

    //             float heightOnAxis = (targetPos - new Vector3(0f, 0f, GroundMinCorner.z)).magnitude;

    //             float fieldOfView = 2.0f * Mathf.Rad2Deg *
    //                              Mathf.Atan(heightOnAxis / vecToTarget.magnitude);
    //             Debug.Log(fieldOfView);

    //             Debug.Log("computed aspect1 (w/h):");
    //             float aspect = Mathf.Abs( GroundMaxCorner.x )  / (2f * heightOnAxis);
    //             Debug.Log(aspect);


    //             Debug.Log("computed aspect2 (w/h):");
    //             aspect = Mathf.Abs(GroundMaxCorner.x) / (2 * Mathf.Abs(GroundMaxCorner.z) );
    //             Debug.Log(aspect);


    //             c.fieldOfView = fieldOfView;
    //             c.aspect = aspect;

    //         }

    //         if (c.name == "CameraToCeilingLeft")
    //         {

    //             //Debug.Log("camera pos=");
    //             //Debug.Log(c.transform.position);

    //             Vector3 targetPos = new Vector3(CeilingMinCorner.x / 2f, 0f, 0f);

    //             //Debug.Log("camera targetPos=");
    //             //Debug.Log(targetPos);


    //             //Debug.Log("computed field of vie:");

    //             Vector3 vecToTarget = targetPos - c.transform.position;

    //             float heightOnAxis = (targetPos - new Vector3(0f, 0f, CeilingMinCorner.z)).magnitude;

    //             float fieldOfView = 2.0f * Mathf.Rad2Deg *
    //                              Mathf.Atan(heightOnAxis / vecToTarget.magnitude);
    //             Debug.Log(fieldOfView);

    //             Debug.Log("computed aspect1 (w/h):");
    //             float aspect = Mathf.Abs( CeilingMinCorner.x ) / ( 2f * heightOnAxis );
    //             Debug.Log(aspect);


    //             Debug.Log("computed aspect2 (w/h):");
    //             aspect = Mathf.Abs( CeilingMinCorner.x) / (2 * Mathf.Abs(CeilingMinCorner.z));
    //             Debug.Log(aspect);


    //             c.fieldOfView = fieldOfView;
    //             c.aspect = aspect;
    //         }



    //         if (c.name == "CameraToCeilingRight")
    //         {

    //             //Debug.Log("camera pos=");
    //             //Debug.Log(c.transform.position);


    //             Vector3 targetPos = (  (CeilingMinCorner + CeilingMaxCorner) / 2f + new Vector3(CeilingMaxCorner.x, 0f, 0f) ) /2f; 
    //             //Debug.Log("camera targetPos=");
    //             //Debug.Log(targetPos);


    //             //Debug.Log("computed field of vie:");

    //             Vector3 vecToTarget = targetPos - c.transform.position;

    //             float heightOnAxis = (targetPos - new Vector3(0f, 0f, CeilingMaxCorner.z)).magnitude;

    //             float fieldOfView = 2.0f * Mathf.Rad2Deg *
    //                              Mathf.Atan(heightOnAxis / vecToTarget.magnitude);
    //             Debug.Log(fieldOfView);

    //             Debug.Log("computed aspect1 (w/h):");
    //             float aspect = Mathf.Abs(CeilingMaxCorner.x) / (2f * heightOnAxis);
    //             Debug.Log(aspect);


    //             Debug.Log("computed aspect2 (w/h):");
    //             aspect = Mathf.Abs(CeilingMaxCorner.x) / (2 * Mathf.Abs(CeilingMaxCorner.z));



    //             c.fieldOfView = fieldOfView;
    //             c.aspect = aspect;
    //         }


    //         //if (c.name == "CameraToFrontWall")
    //         //{
    //         //    //Debug.Log("camera pos=");
    //         //    //Debug.Log(c.transform.position);


    //         //    Vector3 targetPos = new Vector3(0.0f, (CeilingMaxCorner.y + GroundMinCorner.y) / 2.0f, CeilingMaxCorner.z);
    //         //    //Debug.Log("camera targetPos=");
    //         //    //Debug.Log(targetPos);


    //         //    //Debug.Log("computed field of view:");

    //         //    Vector3 vecToTarget = targetPos - c.transform.position;

    //         //    float fieldOfView = 2.0f * Mathf.Rad2Deg * Mathf.Atan((CeilingMaxCorner.y - GroundMinCorner.y) / 4.0f / vecToTarget.magnitude);
    //         //    //Debug.Log(fieldOfView);

    //         //    //Debug.Log("computed aspect:");
    //         //    float aspect = (GroundMaxCorner.x - GroundMinCorner.x) / (CeilingMaxCorner.y - GroundMinCorner.y);
    //         //   // Debug.Log(aspect);

    //         //    c.fieldOfView = fieldOfView;
    //         //    c.aspect = aspect;
    //         //}

    //         //if (c.name == "CameraToLeftWall")
    //         //{
    //         //    //Debug.Log("camera pos=");
    //         //    //Debug.Log(c.transform.position);


    //         //    Vector3 targetPos = new Vector3(GroundMinCorner.x, (CeilingMaxCorner.y + GroundMinCorner.y) / 2.0f, 0.0f);

    //         //    //Debug.Log("camera targetPos=");
    //         //    //Debug.Log(targetPos);


    //         //    //Debug.Log("computed field of view:");

    //         //    Vector3 vecToTarget = targetPos - c.transform.position;

    //         //    float fieldOfView = 2.0f * Mathf.Rad2Deg * Mathf.Atan((CeilingMaxCorner.y - GroundMinCorner.y) / 2.0f / vecToTarget.magnitude);
    //         //  //  Debug.Log(fieldOfView);

    //         //   // Debug.Log("computed aspect:");
    //         //    float aspect = (GroundMaxCorner.z - GroundMinCorner.z) / (CeilingMaxCorner.y - GroundMinCorner.y);
    //         //   // Debug.Log(aspect);

    //         //    c.fieldOfView = fieldOfView;
    //         //    c.aspect = aspect;
    //         //}


    //         //if (c.name == "CameraToRightWall")
    //         //{

    //         //    //Debug.Log("camera pos=");
    //         //    //Debug.Log(c.transform.position);


    //         //    Vector3 targetPos = new Vector3(GroundMaxCorner.x, (CeilingMaxCorner.y + GroundMinCorner.y) / 2.0f, 0.0f);

    //         //    //Debug.Log("camera targetPos=");
    //         //    //Debug.Log(targetPos);

    //         //    //Debug.Log("computed field of vie:");

    //         //    Vector3 vecToTarget = targetPos - c.transform.position;

    //         //    float fieldOfView = 2.0f * Mathf.Rad2Deg * Mathf.Atan((CeilingMaxCorner.y - GroundMinCorner.y) / 2.0f / vecToTarget.magnitude);
    //         //    Debug.Log(fieldOfView);

    //         //    Debug.Log("computed  aspect:");
    //         //    float aspect = (GroundMaxCorner.z - GroundMinCorner.z) / (CeilingMaxCorner.y - GroundMinCorner.y);
    //         //    Debug.Log(aspect);

    //         //    c.fieldOfView = fieldOfView;
    //         //    c.aspect = aspect;
    //         //}


    //     } // for all cameras


    //     //// check if the camera properties are changed
    //     //Debug.Log("Camera Update Check");

    //     //foreach (Camera c in Camera.allCameras)
    //     //{
    //     //    Debug.Log("game name=" + c.gameObject.name);
    //     //    Debug.Log("name=" + c.name);
    //     //    Debug.Log("vertical field of view ="); Debug.Log(c.fieldOfView);
    //     //    Debug.Log("aspect (=w/h) = "); Debug.Log(c.aspect);


    //     //} // 

    // } // InitializeCameras()


    //Camera[] myCams = new Camera[4];


    // void mapCameraToDisplay()
    // {
    //     //Loop over Connected Displays
    //     for (int i = 0; i < Display.displays.Length; i++)
    //     {
    //         myCams[i].targetDisplay = i ; //Set the Display in which to render the camera to
    //         Display.displays[i].Activate(); //Enable the display
    //     }
    // }

    // void OnDisplaysUpdated()
    // {
    //     Debug.Log("New Display Connected. Show Display Option Menu....");
    // }


    // private void Awake()
    // {
    //     //Get Main Camera
    //     myCams[0] = GameObject.FindGameObjectWithTag("MainCamera").GetComponent<Camera>();

    //     //Find All other Cameras
    //     myCams[1] = GameObject.Find("CameraToGroundLeft").GetComponent<Camera>();
    //     myCams[2] = GameObject.Find("CameraToGroundRight").GetComponent<Camera>();
    //     //myCams[3] = GameObject.Find("Camera4").GetComponent<Camera>();

    //     //Call function when new display is connected
    //     Display.onDisplaysUpdated += OnDisplaysUpdated;

    //     //Map each Camera to a Display
    //     mapCameraToDisplay();
    //     //Screen.SetResolution( 2 * 1920, 1080, false); // false = no full screen
    //     //if (Display.displays.Length > 1)
    //     //{
    //     //   // Display.displays[0].SetRenderingResolution(1920, 1080);
    //     //    Display.displays[0].SetParams( 1920, 1080, 0,0);
    //     //    Display.displays[1].Activate();
    //     //    //Display.displays[1].SetRenderingResolution(1920, 1080);
    //     //    Display.displays[1].SetParams(1920, 1080, 0, 1080);
    //     //}
    // }


    // Use this for initialization
    private void Start () 
	{


        //BoidsNum = GetComponent<SimpleBoids>().BoidsNum;
        //GroundMaxCorner = GetComponent<SimpleBoids>().GroundMaxCorner;
        //GroundMinCorner = GetComponent<SimpleBoids>().GroundMinCorner;

        //CeilingMaxCorner = GetComponent<SimpleBoids>().CeilingMaxCorner;
        //CeilingMinCorner = GetComponent<SimpleBoids>().CeilingMinCorner;

        GroundMaxCorner = _boids.GroundMaxCorner;
        GroundMinCorner = _boids.GroundMinCorner;

        CeilingMaxCorner = _boids.CeilingMaxCorner;
        CeilingMinCorner = _boids.CeilingMinCorner;


        //Screen.SetResolution( sum of width of all displays, height, false)

       
        // InitializeCameras();



        // Create Vector2 vertices
        float unitRadius = 1f; // radius = 1m
        Vector2[] vertices = Triangulator.FindPointsOnCircle( unitRadius); // 2D circle on xz plane

        // Use the triangulator to get indices for creating triangles
        Triangulator tr = new Triangulator(vertices);
        indices = tr.Triangulate();

        vertices3D = new Vector3[vertices.Length];

        for (int i=0; i < vertices.Length; i++)
        {

            vertices3D[i] = new Vector3(vertices[i].x, 0.0f, vertices[i].y);

        }

        _instanceMeshCircle = new Mesh();

        _instanceMeshCircle.vertices = vertices3D;
        _instanceMeshCircle.triangles = indices;
        _instanceMeshCircle.RecalculateNormals();
        _instanceMeshCircle.RecalculateBounds();




        // for debugging

        // _instanceMesh = GetComponent < Mesh > (); // get the Mesh component for the current gameboject

        _argsBuffer = new ComputeBuffer(
			1, 
			_args.Length * sizeof(uint), 
			ComputeBufferType.IndirectArguments
		);


        //float[] GroundMaxCornerF = new float[3];
        //float[] GroundMinCornerF = new float[3];

        //float[] CeilingMaxCornerF = new float[3];
        //float[] CeilingMinCornerF = new float[3];

        // GroundMaxCornerF[0] = GroundMaxCorner[0];
        // GroundMinCornerF[1] = GroundMinCorner[1];

        // GroundMaxCornerF[2] = GroundMaxCorner[2];


        //  CeilingMinCornerF[0] = CeilingMinCorner[0];
        //  CeilingMinCornerF[1] = CeilingMinCorner[1];

        // CeilingMinCornerF[2] = CeilingMinCorner[2];

        //https://answers.unity.com/questions/979080/how-to-pass-an-array-to-a-shader.html
        //_instanceMaterial.SetFloatArray("GroundMaxCorner", GroundMaxCornerF);
        //_instanceMaterial.SetFloatArray("GroundMinCorner", GroundMinCornerF);

        // _instanceMaterial.SetFloatArray("CeilingMaxCorner", CeilingMaxCornerF);
        // _instanceMaterial.SetFloatArray("CeilingMinCorner", CeilingMinCornerF);

        // // Shader vectors are always Vector4s.
        // But the value here is converted to a Vector3.
        //Vector3 value = Vector3.one;
        //Renderer renderer = GetComponent<Renderer>();
        //renderer.material.SetVector("_SomeVariable", value);

        // Use SetVector() rather than setFloatArray
        _instanceMaterial.SetVector("GroundMaxCorner",  GroundMaxCorner);
        _instanceMaterial.SetVector("GroundMinCorner", GroundMinCorner);

         _instanceMaterial.SetVector("CeilingMaxCorner", CeilingMaxCorner);
         _instanceMaterial.SetVector("CeilingMinCorner", CeilingMinCorner);

       // _instanceMaterial.SetFloat("Use3DBoids", Use3DBoids);

        _instanceMaterial.SetBuffer("_BoidBuffer", _boids.BoidBuffer); // _boids.BoidBuffer is ceated in SimpleBoids.cs
    } // Start()
	
	// Update is called once per frame
	public void Update () 
	{
		RenderInstancedMesh();              
       
    }

	private void OnDestroy()
	{
		if(_argsBuffer == null) return;
		_argsBuffer.Release();
		_argsBuffer = null;
	}

	private void RenderInstancedMesh()
	{
        //var boidArray = new BoidData[BoidsNum];

        _meshNo = (int) _boids._meshSetting.MeshNo;
        _scale = _boids._meshSetting.Scale;

        if ( _meshNo == 1) // use 2D boids => creat the mesh in a script
        {
           
            _instanceMesh = _instanceMeshCircle;

            _instanceMaterial.SetVector("_Scale", new Vector3(_scale, _scale, _scale) );
        }

        if ( _meshNo == 2 ) // use the mesh assigned in the inspector
        {
            _instanceMesh = _instanceMeshSphere;

            _instanceMaterial.SetVector("_Scale", new Vector3(_scale, _scale, _scale));




            //  _instanceMesh.RecalculateNormals();
            // _instanceMesh.RecalculateBounds();


        }

        if (_meshNo == 3) // use the mesh assigned in the inspector
        {
            _instanceMesh = _instanceMeshHuman1;

            _instanceMaterial.SetVector("_Scale", new Vector3(_scale, _scale, _scale));
        

            //  _instanceMesh.RecalculateNormals();
            // _instanceMesh.RecalculateBounds();


        }
        if (_meshNo == 4) // use the mesh assigned in the inspector
        {
            _instanceMesh = _instanceMeshHuman2;

            _instanceMaterial.SetVector("_Scale", new Vector3(_scale, _scale, _scale));
           

            //  _instanceMesh.RecalculateNormals();
            // _instanceMesh.RecalculateBounds();


        }

        if (_meshNo == 5) // use the mesh assigned in the inspector
        {
            _instanceMesh = _instanceMeshHuman3;

            _instanceMaterial.SetVector("_Scale", new Vector3(_scale, _scale, _scale) );

            //  _instanceMesh.RecalculateNormals();
            // _instanceMesh.RecalculateBounds();


        }
        if (_meshNo == 6) // use the mesh assigned in the inspector
        {
            _instanceMesh = _instanceMeshHuman4;

            _instanceMaterial.SetVector("_Scale", new Vector3(_scale, _scale, _scale));

      

            //  _instanceMesh.RecalculateNormals();
            // _instanceMesh.RecalculateBounds();


        }

        //Debug.Log("number of indices=");
        //Debug.Log(_instanceMesh.GetIndexCount(0));


        // Indirect 
        numIndices = _instanceMesh ? _instanceMesh.GetIndexCount(0) : 0;
        //GetIndexCount(submesh = 0)



        // check if _boids.BoidBuffer is not null
        if (_boids.BoidBuffer == null) return; // nothing to render; 

        //  _instanceMaterial.SetBuffer("_BoidBuffer", _boids.BoidBuffer);


        // _boids.BoidBuffer.GetData(boidArray);
        // https://unity3d.com/kr/learn/tutorials/topics/graphics/gentle-introduction-shaders

        //Debug.Log("Current Num of Boids=");
        //Debug.Log(_boids.BoidsNum);

        _args[0] = numIndices;
        _args[1] = (uint)_boids.m_BoidsNum;

        _argsBuffer.SetData(_args);



        Graphics.DrawMeshInstancedIndirect(
			_instanceMesh,
			0,
			_instanceMaterial,
			new Bounds(_boids.RoomCenter, _boids.RoomSize), 
			_argsBuffer
		);


        // reading from the buffer written by regular shaders
        //https://gamedev.stackexchange.com/questions/128976/writing-and-reading-computebuffer-in-a-shader
       
        // _boids.BoidBuffer.GetData(boidArray);




    }

    //  public struct BoidData
    //  {
    //     public Vector2 Position; // the position of a boid center; float x and float y
    //     public Vector2 Scale; // the scale factors of x and z directions
    //     public float Angle; // the head angle of a boid: from 0 to 2 *PI
    //     public float Speed;            // the speed of a boid
    //     public Vector4 Color;         // RGBA color
    //     public Vector2 SoundGrain; // soundGrain = (freq, amp)
    //      public float Duration;     // duration of a boid each frame
    //      public int  WallNo;      // indicates whether the boid is on ground or on ceiling
    //   }

}
