
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


export async function fetchRandomAvatarImage(): Promise<string> {
  try {
    const response = await fetch('https://thispersondoesnotexist.com');
    if (!response.ok) {
      throw new Error(`Failed to fetch image: ${response.status}`);
    }
    const blob = await response.blob();
    const value = URL.createObjectURL(blob);
    console.log('Fetched image: ', value);
    return value;
  } catch (error) {
    console.error('Error fetching image: ', error);
    throw error;
  }
}


export function generateRandomNumbers(count: number, min: number, max: number): number[] {
  const result: number[] = [];

  for (let i = 0; i < count; i++) {
    result.push(Math.floor(Math.random() * (max - min + 1)) + min);
  }

  return result;
}

export function getRandomInt(min: number, max: number) {
  return Math.floor(Math.random() * (max - min + 1)) + min
}