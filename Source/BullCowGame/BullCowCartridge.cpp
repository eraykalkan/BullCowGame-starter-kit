// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);

    SetupGame(); // setting up the game

    //PrintLine(TEXT("The Hidden Word is: %s. \nit is %i letters long"), *HiddenWord,HiddenWord.Len());
    //PrintLine(FString::Printf(TEXT("The Hidden Word is: %s"),*HiddenWord)); // debug line
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    // if game is over then do ClearScreen() and SetupGame() the game.
    // else Checking the player guess

    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(Input);

    }

}

void UBullCowCartridge::SetupGame()
{
    // welcoming the player
    PrintLine(TEXT("Hi There! Welcome to Bulls Cows Game"));

    //HiddenWord = TEXT("cakes"); //set the hidden word
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    PrintLine(TEXT("The Hidden Word is: %s. \nit is %i letters long"), *HiddenWord, HiddenWord.Len());

    //Set lives
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word.."), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives to guess."), Lives);
    PrintLine(TEXT("Type in your guess. \nPress enter key to continue.."));

    //PrintLine(TEXT("The number of hidden words : %i"), Words.Num());

    /*for (int32 Index = 0; Index < Words.Num(); Index++)
    {
        PrintLine(TEXT("%s"), *Words[Index]);
    }*/

    /*const TCHAR HW[] = TEXT("plums");
    PrintLine(TEXT("Character 1 of the hidden word is: %c"), HiddenWord[0]); //print c
    PrintLine(TEXT("The 4th character of the HW is: %c"), HW[3]); //print m
    */
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nGame is over. Press ENTER to play again.."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{

    if (Guess == HiddenWord)
    {
        PrintLine("You win!");
        EndGame();
        return;
    }


    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The hidden word is %i letters long"), HiddenWord.Len());
        PrintLine(TEXT("Sorry, try guessing again! \nRemaining Lives: %i"), Lives);
        return;
    }


    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again"));
        return;
    }


    --Lives;
    PrintLine(TEXT("You lost a life!"));
    
    if (Lives <= 0)
    {      
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("\nThe hidden word was %s"), *HiddenWord);
        EndGame();
        return;
    }

    FBullCowCount Score =  GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    PrintLine(TEXT("Guess again, you have %i lives left."), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{

    /*for (int32 Index = 0 , Comparison = Index + 1 ; Comparison < Word.Len(); Comparison++)
    {
        if (Word[Index] == Word[Comparison])
        {
            return false;
        }
    }*/

    //abcd

    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }          
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() > 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;

}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{

    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }
        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;

}