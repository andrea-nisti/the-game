import { Box } from '@chakra-ui/react';
import React from 'react';
import { Player, TableCard } from '../components/GameComponents';
import {getRandomInt } from "../utils/Common"
import GameCard from '../components/GameCard';

interface GameProps {
  title: string;
}

interface GameState {
  userHand: GameCard[];
}



class Game extends React.Component<GameProps, GameState> {
  constructor(props: GameProps) {
    super(props);
    this.state = {
      userHand: [
        new GameCard({ value: getRandomInt(1, 99), isActive: false }),
        new GameCard({ value: getRandomInt(1, 99), isActive: false }),
        new GameCard({ value: getRandomInt(1, 99), isActive: false }),
        new GameCard({ value: getRandomInt(1, 99), isActive: false }),
        new GameCard({ value: getRandomInt(1, 99), isActive: false }),
        new GameCard({ value: getRandomInt(1, 99), isActive: false })
      ],
    };
  }

  render() {
    return (
      <Box
        // sx={{ border: '10px solid red' }}
        display='flex'
        flexDirection='column-reverse'
        height='94vh'
        width='98.5vw'
        gap='100px'>

        <Box
          // sx={{ border: '10px solid white' }}
          justifyContent='center'
          display='flex'
          flexDirection='row'
          gap='10px'
          marginBottom='100px'>

          {this.state.userHand.map((card: GameCard) => (
            <GameCard isActive={false} value={getRandomInt(1, 99)} />

          ))}
        </Box>

        <Box
          // sx={{ border: '10px solid white' }}
          justifyContent='center'
          alignItems='center'
          display='flex'
          flexDirection='column'
          gap='10px'>

          <Box display='flex' flexDirection='row' gap='10px'>
            <TableCard value={1} direction="up" />
            <TableCard value={1} direction="up" />
          </Box>
          <Box display='flex' flexDirection='row' gap='10px'>
            <TableCard value={99} direction="down" />
            <TableCard value={99} direction="down" />
          </Box>
        </Box>
        <Box
          // sx={{ border: '10px solid white' }}
          justifyContent='center'
          alignItems='flex-end'
          display='flex'
          flexDirection='row'
          gap='50px'>
          <Player name="Prosepio Finacchioni" cardNumber={3} />
          <Player name="Marchionne Pampalone" cardNumber={3} />
          <Player name="Frano Sticchiano" cardNumber={3} />
        </Box>
      </Box>
    );
  }
}

export default Game;