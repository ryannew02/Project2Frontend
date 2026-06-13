import React, { useRef, useMemo, useEffect, useCallback } from 'react';
import { Canvas, useFrame } from '@react-three/fiber';
import * as THREE from 'three';
import { OrbitControls } from '@react-three/drei';

function InstancedCubes({ dataRef, countRef }) {
  const meshRef = useRef();
  const tempObject = useMemo(() => new THREE.Object3D(), []);

  useFrame((state) => {
    const count = countRef.current;
    if (!meshRef.current || count === 0) return;

    meshRef.current.count = count;

    for (let i = 0; i < count; i++) {
      const x = dataRef.current[i * 3];
      const y = dataRef.current[i * 3 + 1];
      const z = 0;
      tempObject.position.set(x, y, z);
      tempObject.rotation.y = state.clock.getElapsedTime() * 2.0;
      tempObject.updateMatrix();
      meshRef.current.setMatrixAt(i, tempObject.matrix);
    }
    meshRef.current.instanceMatrix.needsUpdate = true;
  });

  return (
    <instancedMesh ref={meshRef} args={[null, null, 100000]}>
      <boxGeometry args={[0.5, 0.5, 0.5]} />
      <meshStandardMaterial color="cyan" />
    </instancedMesh>
  );
}

// No props at all — Scene mounts once and never re-renders
const Scene = React.memo(() => {
  const dataRef = useRef([]);
  const countRef = useRef(0);

  // Expose refs on window so App can write to them without causing re-renders
  window._dataRef = dataRef;
  window._countRef = countRef;

  return (
    <Canvas
      style={{
        position: 'absolute',
        top: '50px',
        left: '50px',
        right: '50px',
        bottom: '50px',
        width: 'calc(100vw - 100px)',
        height: 'calc(100vh - 100px)',
        background: '#ebebeb'
      }}
      camera={{ position: [50, 0, 150], fov: 60 }}
    >
      <ambientLight intensity={0.6} />
      <directionalLight position={[10, 20, 15]} intensity={1.2} />
      <InstancedCubes dataRef={dataRef} countRef={countRef} />
      <OrbitControls target={[50, 50, 50]} />
    </Canvas>
  );
});

export default function App() {
  const refreshData = useCallback(() => {
    fetch('http://localhost:3000/api/random-array')
      .then((res) => res.json())
      .then((data) => {
        // Write directly into the refs — zero React re-renders
        window._dataRef.current = data;
        window._countRef.current = Math.floor(data.length / 3);
      })
      .catch((err) => console.error("Fetch error:", err));
  }, []);

  useEffect(() => { refreshData(); }, [refreshData]);

  return (
    <div style={{ width: '100vw', height: '100vh', background: '#000', position: 'relative', boxSizing: 'border-box' }}>

      <button
        onClick={refreshData}
        style={{
          position: 'absolute',
          top: '10px',
          left: '50%',
          transform: 'translateX(-50%)',
          zIndex: 10,
          padding: '12px 24px',
          background: 'cyan',
          border: 'none',
          borderRadius: '4px',
          cursor: 'pointer',
          fontWeight: 'bold',
          color: '#111'
        }}
      >
        Regenerate C++ Data
      </button>

      <Scene />

    </div>
  );
}

// export default function App() {
//   return (
//     <div style={{ width: '100%', height: '100%' }}>
//       <Canvas>
//         <PerspectiveCamera makeDefault position={[0, 0, 50]} />
//         <ambientLight intensity={0.5} />
//         <InstancedCubes />
//         <OrbitControls makeDefault />
//       </Canvas>
//     </div>
//   );
// }



////////////////////////////////////
// simple cube rotational movement
// import React, { useRef } from 'react';
// import { Canvas, useFrame } from '@react-three/fiber';
// import { OrbitControls, PerspectiveCamera } from '@react-three/drei';

// function SpinningCube() {
//   const meshRef = useRef();
//   useFrame((state, delta) => {
//     meshRef.current.rotation.x += delta;
//     meshRef.current.rotation.y += delta * 0.5;
//   });

//   return (
//     <mesh ref={meshRef}>
//       <boxGeometry args={[2, 2, 2]} />
//       <meshStandardMaterial color="mediumpurple" />
//     </mesh>
//   );
// }

// export default function App() {
//   return (
//     <div style={{ width: '100vw', height: '100vh' }}>
//       <Canvas>
//         {/* Explicitly setting the camera helps prevent "blank screen" issues */}
//         <PerspectiveCamera makeDefault position={[0, 0, 5]} />
//         <ambientLight intensity={0.5} />
//         <directionalLight position={[10, 10, 5]} intensity={1} />
//         <SpinningCube />
//         <OrbitControls />
//       </Canvas>
//     </div>
//   );
// }

////////////////////////////////////
// Simple cube axis movement

// import React, { useRef } from 'react';
// import { Canvas, useFrame } from '@react-three/fiber';
// import { OrbitControls, PerspectiveCamera } from '@react-three/drei';

// function MovingCube() {
//   const meshRef = useRef();

//   useFrame((state) => {
//     // We use the clock's elapsed time to move the cube
//     // Math.sin oscillates between -1 and 1, creating a back-and-forth movement
//     const t = state.clock.getElapsedTime();
//     meshRef.current.position.x = Math.sin(t) * 3; // Moves between -3 and +3 on the X-axis
//   });

//   return (
//     <mesh ref={meshRef}>
//       <boxGeometry args={[1, 1, 1]} />
//       <meshStandardMaterial color="hotpink" />
//     </mesh>
//   );
// }

// export default function App() {
//   return (
//     <div style={{ width: '100vw', height: '100vh' }}>
//       <Canvas>
//         <PerspectiveCamera makeDefault position={[0, 0, 8]} />
//         <ambientLight intensity={0.5} />
//         <directionalLight position={[10, 10, 5]} intensity={1} />
//         <MovingCube />
//         <OrbitControls />
//       </Canvas>
//     </div>
//   );
// }

////////////////////////////////////////
// Simple Cube array

// import React, { useRef } from 'react';
// import { Canvas, useFrame } from '@react-three/fiber';
// import { OrbitControls, PerspectiveCamera } from '@react-three/drei';


// const cubePositions = [
//   { id: 1, x: 0, y: 2, z: 1 },
//   { id: 2, x: -2, y: 1, z: 0 },
//   { id: 3, x: 0,  y: 0, z: 0 },
//   { id: 4, x: 2,  y: -1, z: 0 },
//   { id: 5, x: 4,  y: 0, z: 0 },
// ];

// function Cube({ position }) {
//   return (
//     <mesh position={position}>
//       <boxGeometry args={[1, 1, 1]} />
//       <meshStandardMaterial color="cyan" />
//     </mesh>
//   );
// }

// function MovingCube() {
//   const meshRef = useRef();

//   useFrame((state) => {
//     // We use the clock's elapsed time to move the cube
//     // Math.sin oscillates between -1 and 1, creating a back-and-forth movement
//     const t = state.clock.getElapsedTime();
//     meshRef.current.position.x = Math.sin(t) * 3; // Moves between -3 and +3 on the X-axis
//   });

//   return (
//     <mesh ref={meshRef}>
//       <boxGeometry args={[1, 1, 1]} />
//       <meshStandardMaterial color="hotpink" />
//     </mesh>
//   );
// }

// function MovingCube2() {
//   const meshRef = useRef();

//   useFrame((state) => {
//     // We use the clock's elapsed time to move the cube
//     // Math.sin oscillates between -1 and 1, creating a back-and-forth movement
//     const t = state.clock.getElapsedTime();
//     meshRef.current.position.x = Math.sin(t-1) * 3; // Moves between -3 and +3 on the X-axis
//   });

//   return (
//     <mesh ref={meshRef}>
//       <boxGeometry args={[1, 1, 1]} />
//       <meshStandardMaterial color="hotpink" />
//     </mesh>
//   );
// }

// export default function App() {
//   return (
//     <div style={{ width: '100vw', height: '100vh' }}>
//       <Canvas>
//         <ambientLight intensity={0.5} />
//         {cubePositions.map((cube) => (
//           <Cube 
//             key={cube.id} 
//             position={[cube.x, cube.y, cube.z]} 
//           />
//         ))}
//         <OrbitControls />
//       </Canvas>
//     </div>
//   );
// }






/////////////////////////////////////////
//
//  Simple window that gives count when clicked
//
/////////////////////////////////////////


// // import React from "react";
// // import "./style.css";
// import React, { useState } from "react";

// export default function App() {
//   // Allocate tracked memory. Initial state is 0.
//   const [number, setNumber] = useState(0);

//   // An internal execution function to mutate state
//   function handleIncrement() {
//     setNumber(number + 1); 
//   }

//   return (
//     <div style={{ padding: "20px" }}>
//       {/* 1. Display the state variable */}
//       <h1>The count is: {number}</h1>

//       {/* 2. Bind the execution function to a user event */}
//       <button onClick={handleIncrement}>
//         Change Number
//       </button>
//     </div>
//   );
// }

////////////////////////////////////////////////////
////////////////////////////////////////////////////

//           Default App that came with it        //

////////////////////////////////////////////////////
////////////////////////////////////////////////////


// import { useState } from 'react'
// import reactLogo from './assets/react.svg'
// import viteLogo from './assets/vite.svg'
// import heroImg from './assets/hero.png'
// import './App.css'

// function App() {
//   const [count, setCount] = useState(0)

//   return (
//     <>
//       <section id="center">
//         <div className="hero">
//           <img src={heroImg} className="base" width="170" height="179" alt="" />
//           <img src={reactLogo} className="framework" alt="React logo" />
//           <img src={viteLogo} className="vite" alt="Vite logo" />
//         </div>
//         <div>
//           <h1>Get started</h1>
//           <p>
//             Edit <code>src/App.jsx</code> and save to test <code>HMR</code>
//           </p>
//         </div>
//         <button
//           type="button"
//           className="counter"
//           onClick={() => setCount((count) => count + 1)}
//         >
//           Count is {count}
//         </button>
//       </section>

//       <div className="ticks"></div>

//       <section id="next-steps">
//         <div id="docs">
//           <svg className="icon" role="presentation" aria-hidden="true">
//             <use href="/icons.svg#documentation-icon"></use>
//           </svg>
//           <h2>Documentation</h2>
//           <p>Your questions, answered</p>
//           <ul>
//             <li>
//               <a href="https://vite.dev/" target="_blank">
//                 <img className="logo" src={viteLogo} alt="" />
//                 Explore Vite
//               </a>
//             </li>
//             <li>
//               <a href="https://react.dev/" target="_blank">
//                 <img className="button-icon" src={reactLogo} alt="" />
//                 Learn more
//               </a>
//             </li>
//           </ul>
//         </div>
//         <div id="social">
//           <svg className="icon" role="presentation" aria-hidden="true">
//             <use href="/icons.svg#social-icon"></use>
//           </svg>
//           <h2>Connect with us</h2>
//           <p>Join the Vite community</p>
//           <ul>
//             <li>
//               <a href="https://github.com/vitejs/vite" target="_blank">
//                 <svg
//                   className="button-icon"
//                   role="presentation"
//                   aria-hidden="true"
//                 >
//                   <use href="/icons.svg#github-icon"></use>
//                 </svg>
//                 GitHub
//               </a>
//             </li>
//             <li>
//               <a href="https://chat.vite.dev/" target="_blank">
//                 <svg
//                   className="button-icon"
//                   role="presentation"
//                   aria-hidden="true"
//                 >
//                   <use href="/icons.svg#discord-icon"></use>
//                 </svg>
//                 Discord
//               </a>
//             </li>
//             <li>
//               <a href="https://x.com/vite_js" target="_blank">
//                 <svg
//                   className="button-icon"
//                   role="presentation"
//                   aria-hidden="true"
//                 >
//                   <use href="/icons.svg#x-icon"></use>
//                 </svg>
//                 X.com
//               </a>
//             </li>
//             <li>
//               <a href="https://bsky.app/profile/vite.dev" target="_blank">
//                 <svg
//                   className="button-icon"
//                   role="presentation"
//                   aria-hidden="true"
//                 >
//                   <use href="/icons.svg#bluesky-icon"></use>
//                 </svg>
//                 Bluesky
//               </a>
//             </li>
//           </ul>
//         </div>
//       </section>

//       <div className="ticks"></div>
//       <section id="spacer"></section>
//     </>
//   )
// }

// export default App
