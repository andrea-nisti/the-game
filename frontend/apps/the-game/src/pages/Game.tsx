import { Box, Card } from '@chakra-ui/react';
import React from 'react';
import GameCard from '../components/GameCard';
import UserDeck from '../components/UserDeck';
import CompanionTab from '../components/CompanionTab';
import Player from '../components/Player';
import { createCompanions, createInitialUserHand, createTableCards } from '../utils/GameUtils';
import GameTable from '../components/Table';

interface GameProps {
  title: string;
}

interface GameState {
  userHand: GameCard[];
  companions: Player[];
  // tableCards: TableCard[];
  activeUserCardIndex: number;
  activeUserCardValue: number;
  activeTableCardIndex: number;
  activeTableCardValue: number
}


class Game extends React.Component<GameProps, GameState> {
  constructor(props: GameProps) {
    super(props);
    this.state = {
      companions: createCompanions(['Prosepio Doroni', 'Prosepio dei Finacchioni', 'Madonnina Salutare']),
      userHand: createInitialUserHand(6),
      // tableCards: createTableCards(6),
      activeUserCardIndex: -1,
      activeUserCardValue: -1,
      activeTableCardIndex: -1,
      activeTableCardValue: -1,
    }
  }

  private onPlayClick = () => {
    console.log('VALUE USER CARD: ', this.state.activeUserCardIndex)
    console.log('VALUE TABLE CARD: ', this.state.activeUserCardIndex)

    console.log('this.state.userDeck.state.cards')
  }

  private onUserCardClick = (index: number) => {
    console.log('INDEX USER CARD: ', this.state.activeUserCardIndex)
    console.log('VALUE USER CARD: ', this.state.activeUserCardValue)
    this.setState({ activeUserCardIndex: index, activeUserCardValue: this.state.userHand[index].state.value })
  }

  private onTableCardClick = (index: number) => {
    this.setState({ activeTableCardIndex: index, activeTableCardValue: this.state.userHand[index].state.value })
  }

  render() {
    return (
      <Box
        // sx={{ border: '10px solid red' }}
        display='flex'
        flexDirection='column-reverse'
        height='94vh'
        width='98.5vw'
        gap='6vh'>
          
        <UserDeck cards={this.state.userHand} activeIndex={this.state.activeUserCardIndex} onCardClick={(index) => this.onUserCardClick(index)} />

        <GameTable />

        <CompanionTab players={this.state.companions} />

      </Box>
    );
  }
}

export default Game;
