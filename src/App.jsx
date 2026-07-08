import React, { useState } from 'react';

const WEIGHT_PER_PASSENGER = 190; // lbs average

export default function App() {
  const [addressInput, setAddressInput] = useState('');
  const [addresses, setAddresses] = useState([]);
  const [passengers, setPassengers] = useState(1);
  const [result, setResult] = useState(null);
  const [error, setError] = useState(null);
  const [loading, setLoading] = useState(false);
  const [devMode, setDevMode] = useState(false);
  const [loadTime, setLoadTime] = useState(null);
  // Too add, Addresses for Vertiport locations, Brightline/MCO airport, one at I-4, Wekiwa Springs, FL 32779, and one at Turkey lake turnpike rest stop
  const testAddresses = 
  [
    '10236 Willowemac Ct',
     '1350 Sasoon Ave',
      '5884 Grand Canyon Dr',
       '5641 Curlew Dr',
        '2280 Pebblewood Dr',
         '504 Lake Bridge Ln',
          '504 Lake Bridge Ln',
           '5398 N Orange Blossom Trl',
            '4451 Twinview Ln',
             '1189 Monteagle Cir'
            ]
  // Add address to the list
  const handleAddAddress = () => {
    const trimmed = addressInput.trim();
    if (!trimmed) return;
    setAddresses([...addresses, trimmed]);
    setAddressInput('');
  };
  
  const handleDevAddAddress = () => {
    const devTestAddress = testAddresses[Math.floor(Math.random() * testAddresses.length)]
    setAddresses([...addresses, devTestAddress]);
    setAddressInput(devTestAddress);
  };

  // Remove an address from the list
  const handleRemoveAddress = (index) => {
    setAddresses(addresses.filter((_, i) => i !== index));
  };

  // Send to backend
  const handleCalculate = async (algorithm) => {
    if (addresses.length < 2) {
      setError('Please enter at least 2 addresses');
      return;
    }
    setLoading(true);
    setError(null);
    setResult(null);
    const startTime = performance.now();

    try {
      const res = await fetch('https://oxygen-acetone-tall.ngrok-free.dev/api/route', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ addresses: [" Brightline Orlando Station", ...addresses, " Brightline Orlando Station"], passengers, algorithm }),
      });
      const data = await res.json();
      setResult(data);
      setLoadTime(performance.now() - startTime);
    } catch (err) {
      setError('Failed to reach server');
    } finally {
      setLoading(false);
    }
  };
  const selectedVehicle = result && result.passengers < 3 && result.total_distance_miles < 22 ? { name: 'EHang EH216-S', capacity: 2, range: 22 } : result && result.total_distance_miles < 90 ? { name: 'Wisk Generation 6', capacity: 4, range: 90 } : { name: 'AutoFlight Prosperity I', capacity: 4, range: 155 };
  const estimatedWeight = passengers * WEIGHT_PER_PASSENGER;

  return (
    <div style={styles.page}>

      {/* ── Left Panel ── */}
      <div style={styles.leftPanel}>

        {/* Address Input */}
        <div style={styles.card}>
          {devMode && <div style={styles.label}> Dev Mode Enabled </div>}
          <button style={styles.devButton} onClick={() => setDevMode(!devMode)}>Address</button>
          {!devMode ?
          <input
            style={styles.input}
            type="text"
            placeholder="Enter an address..."
            value={addressInput}
            onChange={(e) => setAddressInput(e.target.value)}
            onKeyDown={(e) => e.key === 'Enter' && handleAddAddress()}
          />
          :
          <input
            style={styles.input}
            type="text"
            placeholder="Dev Mode Enabled..."
            value={addressInput}
            readOnly
            onKeyDown={(e) => e.key === 'Press to Generate Address' && handleDevAddAddress()}
            />
            }
          {!devMode ?
          <button style={styles.enterBtn} onClick={handleAddAddress}>
            Enter
          </button>
          :
          <button style={styles.enterBtn} onClick={handleDevAddAddress}>
            Enter
          </button> 
          }



          {/* Address list preview */}
          {addresses.length > 0 && (
            <div style={styles.addressList}>
              {addresses.map((addr, i) => (
                <div key={i} style={styles.addressChip}>
                  <span>{i + 1}. {addr}</span>
                  <button
                    style={styles.removeBtn}
                    onClick={() => handleRemoveAddress(i)}
                  >✕</button>
                </div>
              ))}
            </div>
          )}

          {/* Passenger selector */}
          <label style={styles.label}>Number of Passengers</label>
          
          <div style={styles.passengerRow}>
            {[1, 2, 3, 4].map((n) => (
              <button
                key={n}
                style={{
                  ...styles.passengerBtn,
                  background: passengers === n ? '#0ff' : '#222',
                  color: passengers === n ? '#000' : '#0ff',
                }}
                onClick={() => setPassengers(n)}
              >
                {n}
              </button>
            ))}
          </div>
        </div>

        {/* Algorithm buttons */}
        <button
          style={styles.algoBtn}
          onClick={() => handleCalculate('splay')}
          disabled={loading}
        >
          {loading ? 'Calculating...' : 'Use Splay Tree to Calculate'}
        </button>

        <button
          style={styles.algoBtn}
          onClick={() => handleCalculate('trie')}
          disabled={loading}
        >
          {loading ? 'Calculating...' : 'Use Trie to Calculate'}
        </button>

        {error && <p style={styles.error}>{error}</p>}
      </div>

      {/* ── Right Panel ── */}
      <div style={styles.rightPanel}>

        {/* Route display */}
        <div style={styles.card}>
          <div style={styles.routeHeader}>
            <span style={styles.label}>Route</span>
            <span style={styles.weightBadge}>
              Est. Passenger Weight: {estimatedWeight} lbs
            </span>
          </div>

          {!result && (
            <p style={styles.placeholder}>
              List of addresses (including vertiports) and route will appear here
            </p>
          )}

          {result && (
            <div>
              {result.route.map((stop, i) => (
                <div key={i} style={styles.routeStop}>
                  <span style={styles.stopNumber}>{i + 1}</span>
                  <div>
                    <div style={styles.stopAddress}>{stop.address}, {stop.city}, FL {stop.zip} </div>
                    {stop.error && (
                      <div style={styles.stopError}>Not found in database</div>
                    )}
                    <div style={styles.stopCoords}>
                      {stop.lat !== 0 && `${stop.lat.toFixed(5)}, ${stop.lon.toFixed(5)}`}
                    </div>
                  </div>
                </div>
              ))}
            </div>
          )}
        </div>

        {/* Dispatch info */}
        <div style={styles.card}>
          <label style={styles.label}>Dispatch Summary</label>
          {!result && (
            <p style={styles.placeholder}>
              Battery estimate will appear after calculating a route
            </p>
          )}
          {result && (
            <div>
              <div style={styles.batteryRow}>
                <span>Total Distance</span>
                <span>{result.total_distance_miles.toFixed(2)} miles</span>
              </div>
              <div style={styles.batteryRow}>
                <span>Passengers</span>
                <span>{result.passengers}</span>
              </div>
              <div style={styles.batteryRow}>
              <span>eVTOL Assigned</span>
              {/*other vehicle availble in production at this time EHang EH216-S (2 pax, 62mph, 22mpc), Wisk Generation 6(4 pax, 126mph, 90mpc), AutoFlight Prosperity I (4 pax, 124mph, 155 miles)*/}
              {/*(result.passengers < 3 & result.total_distance_miles < 22) ? <span>EHang EH216-S</span> : (result.total_distance_miles < 90) ? <span>Wisk Generation 6</span> : <span>AutoFlight Prosperity I</span>*/}
              <span>{selectedVehicle.name}</span>
              </div>
              <div style={styles.batteryRow}>
                <span>Est. Calculated Total Weight</span>
                <span>{estimatedWeight} lbs</span>
              </div>
              <div style={styles.batteryRow}>
              <span>Battery Charge Neccessary</span>
              <span>TBD KWh</span>
              </div>
              <div style={{ ...styles.batteryRow, borderTop: '1px solid #333', paddingTop: '10px', marginTop: '10px' }}>
                <span>Algorithm Used</span>
                <span style={{ color: '#0ff' }}>{result.algorithm || 'N/A'}</span>
              </div>
              {devMode && loadTime !== null && (
              <div style={styles.batteryRow}>
                <span>Load Time (Dev)</span>
                <span style={{ color: '#0ff' }}>{loadTime.toFixed(1)} ms</span>
              </div>)}
            </div>
          )}
        </div>

      </div>
    </div>
  );
}

// ─── Styles ────────────────────────────────────────────────────────────────
const styles = {
  devButton: {
    font: 'inherit',
    fontSize: '12px',
    color: '#888',
    textTransform: 'uppercase',
    letterSpacing: '1px',
    background: 'none',
    border: 'none',
    padding: 0,
    cursor: 'help',
    fontFamily: 'monospace',
    textAlign: 'left',
  },

  page: {
    display: 'flex',
    flexDirection: 'row',
    width: '100vw',
    height: '100vh',
    background: '#111',
    color: '#eee',
    fontFamily: 'monospace',
    boxSizing: 'border-box',
    padding: '20px',
    gap: '20px',
  },
  leftPanel: {
    display: 'flex',
    flexDirection: 'column',
    gap: '12px',
    width: '320px',
    flexShrink: 0,
  },
  rightPanel: {
    display: 'flex',
    flexDirection: 'column',
    gap: '12px',
    flex: 1,
  },
  card: {
    background: '#1a1a1a',
    border: '1px solid #333',
    borderRadius: '8px',
    padding: '16px',
    display: 'flex',
    flexDirection: 'column',
    gap: '10px',
  },
  label: {
    fontSize: '12px',
    color: '#888',
    textTransform: 'uppercase',
    letterSpacing: '1px',
  },
  input: {
    background: '#222',
    border: '1px solid #444',
    borderRadius: '4px',
    color: '#eee',
    padding: '8px 10px',
    fontSize: '14px',
    outline: 'none',
  },
  enterBtn: {
    background: '#0ff',
    color: '#000',
    border: 'none',
    borderRadius: '4px',
    padding: '8px',
    cursor: 'pointer',
    fontWeight: 'bold',
    fontFamily: 'monospace',
  },
  addressList: {
    display: 'flex',
    flexDirection: 'column',
    gap: '6px',
  },
  addressChip: {
    display: 'flex',
    justifyContent: 'space-between',
    alignItems: 'center',
    background: '#222',
    border: '1px solid #333',
    borderRadius: '4px',
    padding: '6px 10px',
    fontSize: '12px',
  },
  removeBtn: {
    background: 'none',
    border: 'none',
    color: '#f66',
    cursor: 'pointer',
    fontSize: '12px',
    padding: '0 4px',
  },
  passengerRow: {
    display: 'flex',
    gap: '8px',
  },
  passengerBtn: {
    width: '40px',
    height: '40px',
    border: '1px solid #0ff',
    borderRadius: '4px',
    cursor: 'pointer',
    fontWeight: 'bold',
    fontFamily: 'monospace',
    fontSize: '16px',
  },
  algoBtn: {
    background: '#1a1a1a',
    border: '1px solid #0ff',
    color: '#0ff',
    borderRadius: '6px',
    padding: '12px',
    cursor: 'pointer',
    fontFamily: 'monospace',
    fontSize: '13px',
  },
  routeHeader: {
    display: 'flex',
    justifyContent: 'space-between',
    alignItems: 'center',
  },
  weightBadge: {
    background: '#222',
    border: '1px solid #444',
    borderRadius: '4px',
    padding: '4px 10px',
    fontSize: '12px',
    color: '#0ff',
  },
  placeholder: {
    color: '#555',
    fontSize: '13px',
    fontStyle: 'italic',
  },
  routeStop: {
    display: 'flex',
    gap: '12px',
    alignItems: 'flex-start',
    padding: '8px 0',
    borderBottom: '1px solid #222',
  },
  stopNumber: {
    background: '#0ff',
    color: '#000',
    borderRadius: '50%',
    width: '22px',
    height: '22px',
    display: 'flex',
    alignItems: 'center',
    justifyContent: 'center',
    fontSize: '11px',
    fontWeight: 'bold',
    flexShrink: 0,
  },
  stopAddress: {
    fontSize: '13px',
    color: '#eee',
  },
  stopCoords: {
    fontSize: '11px',
    color: '#555',
    marginTop: '2px',
  },
  stopError: {
    fontSize: '11px',
    color: '#f66',
    marginTop: '2px',
  },
  batteryRow: {
    display: 'flex',
    justifyContent: 'space-between',
    fontSize: '14px',
    padding: '4px 0',
  },
  error: {
    color: '#f66',
    fontSize: '13px',
    margin: 0,
  },
};