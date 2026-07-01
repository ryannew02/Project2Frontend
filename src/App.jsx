import React, { useState } from 'react';

const WEIGHT_PER_PASSENGER = 190; // lbs average

export default function App() {
  const [addressInput, setAddressInput] = useState('');
  const [addresses, setAddresses] = useState([]);
  const [passengers, setPassengers] = useState(1);
  const [result, setResult] = useState(null);
  const [error, setError] = useState(null);
  const [loading, setLoading] = useState(false);

  // Add address to the list
  const handleAddAddress = () => {
    const trimmed = addressInput.trim();
    if (!trimmed) return;
    setAddresses([...addresses, trimmed]);
    setAddressInput('');
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

    try {
      const res = await fetch('https://oxygen-acetone-tall.ngrok-free.dev/api/route', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ addresses, passengers, algorithm }),
      });
      const data = await res.json();
      setResult(data);
    } catch (err) {
      setError('Failed to reach server');
    } finally {
      setLoading(false);
    }
  };

  const estimatedWeight = passengers * WEIGHT_PER_PASSENGER;

  return (
    <div style={styles.page}>

      {/* ── Left Panel ── */}
      <div style={styles.leftPanel}>

        {/* Address Input */}
        <div style={styles.card}>
          <label style={styles.label}>Address</label>
          <input
            style={styles.input}
            type="text"
            placeholder="Enter an address..."
            value={addressInput}
            onChange={(e) => setAddressInput(e.target.value)}
            onKeyDown={(e) => e.key === 'Enter' && handleAddAddress()}
          />
          <button style={styles.enterBtn} onClick={handleAddAddress}>
            Enter
          </button>

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
          onClick={() => handleCalculate('treap')}
          disabled={loading}
        >
          {loading ? 'Calculating...' : 'Use Treap to Calculate'}
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
              Est. Weight: {estimatedWeight} lbs
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
                    <div style={styles.stopAddress}>{stop.address}</div>
                    {stop.error && (
                      <div style={styles.stopError}>⚠ Not found in database</div>
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

        {/* Battery info */}
        <div style={styles.card}>
          <label style={styles.label}>Battery Charge Needed</label>
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
                <span>Est. Weight</span>
                <span>{estimatedWeight} lbs</span>
              </div>
              <div style={{ ...styles.batteryRow, borderTop: '1px solid #333', paddingTop: '10px', marginTop: '10px' }}>
                <span>Algorithm Used</span>
                <span style={{ color: '#0ff' }}>{result.algorithm || 'N/A'}</span>
              </div>
            </div>
          )}
        </div>

      </div>
    </div>
  );
}

// ─── Styles ────────────────────────────────────────────────────────────────
const styles = {
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