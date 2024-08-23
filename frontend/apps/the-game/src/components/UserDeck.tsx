import React from "react";
import GameCard from "./GameCard";
import { Box } from "@chakra-ui/react";

interface UserDeckProp {
    cards: GameCard[],
    activeIndex?: number,
    selectedCardValue?: number,
    debugBorder?: boolean
    onCardClick: (index: number) => void
}

interface UserDeckState {
    cards: GameCard[],
    activeIndex?: number,
    debugBorder?: boolean
}

export default class UserDeck extends React.Component<UserDeckProp, UserDeckState> {
    constructor(props: UserDeckProp) {
        super(props);

        this.state = {
            cards: props.cards,
            activeIndex: props.activeIndex,
            debugBorder: props.debugBorder
        };
    }

    render() {
        return (

            <Box
                // sx={{ border: { '10px solid white' : '' } }}
                justifyContent='center'
                display='flex'
                flexDirection='row'
                gap='10px'
                marginBottom='100px'>

                {
                    this.state.cards.map((card: GameCard) => (

                        <GameCard
                            key={card.state.index}
                            isActive={card.state.index == this.props.activeIndex}
                            index={card.state.index}
                            value={card.state.value}
                            onClick={() => this.props.onCardClick(card.state.index)}
                        />
                    ))}
            </Box>
        )
    }

}
