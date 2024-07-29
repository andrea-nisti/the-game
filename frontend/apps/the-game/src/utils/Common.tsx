
export async function getStatus(): Promise<String[]> {
    try {
        const apiUrl = 'http://localhost:8000/get_status'
        console.log('Calling backend at ' + apiUrl)
        const response = await fetch(apiUrl, { method: 'GET' });
      if (!response.ok) { throw new Error(`API request failed with status ${response.status}`); }
  
      const data = await response.json();
      console.debug('Fetched status: ' + data['status'])
  
      return data['status'];
  
    } catch (error) {
      console.error('Error fetching items:', error);
      return [];
    }
  };


  export function generateRandomNumbers(count: number, min: number, max: number): number[] {
    const result: number[] = [];
  
    for (let i = 0; i < count; i++) {
      result.push(Math.floor(Math.random() * (max - min + 1)) + min);
    }
  
    return result;
  }