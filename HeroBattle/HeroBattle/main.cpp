#include <iostream>
#include <vector>
#include "fstream"
#include "string"
#include "menu.h"

using namespace std;

class Logs {
private:
    static Logs* instance;
    ofstream log_file;
    int turn;
    Logs() { turn = 1;  }
    ~Logs() {  }
public:
    static Logs* getInstance() {
        if (!instance)
            instance = new Logs;
        return instance;
    }

    void CleanLogs() { log_file.open("LOGS.txt", ios::trunc); log_file.close(); }

    void Logger(string log_info, int id, int points = -1) {
        log_file.open("LOGS.txt", ios::app);
        if (id == 0) { if (points != -1) { log_file << "Player: " << log_info << "[ " << points << " ]" << "\n"; } else { log_file << "Player: " << log_info << "\n"; } }
        else if (id == 1) { if (points != -1) { log_file << "Computer: " << log_info << "[ " << points << " ]" << "\n"; } else { log_file << "Computer: " << log_info << "\n"; } }
        else if (id == 2){ log_file << log_info << turn << "\n"; turn++; }
        else if (id == 3) { log_file << log_info; }
        log_file.close();
    }

};

Logs* Logs::instance = 0;

void LOG(string log_info, int id, int points = -1) {
    Logs* logs = logs->getInstance();
    logs->Logger(log_info, id, points);
}

class Hero
{
    int id;
    bool SHILED; // блокирует 4 ед урона
    bool AttackOpportunity, DodgeOpportunity;
    bool bleeding;
    int HEALTH;
    vector<Hero*> Hero_Skills;
    vector<string> SkillNames;
public:
    Hero() { HEALTH = 20; SHILED = false; AttackOpportunity = false; DodgeOpportunity = false; bleeding = false; }
    virtual void createMagic() {}
    virtual int Magic() { return 0; }

    virtual void createPunch() {}
    virtual int Attack_Punch() { return 0; }

    virtual void createSpecialSkill() {}
    virtual bool Special() { return 0; }

    virtual ~Hero() {}

    vector<Hero*> GetHero() { return Hero_Skills; }
    void SetHeroSkills(vector<Hero*> skills) { Hero_Skills = skills; }

    void SetID(int id_) { id = id_; }

    void ArmorOn() { SHILED = true; LOG("Shield Actived", id); }
    void Heal(int points) { HEALTH += points; LOG("Healed 3 HP", id); }
    int GetHealth() { return HEALTH; }
    void ShieldStatus() { if (SHILED != false) { cout << "Active"; } else cout << "Disable"; }
    void Attacked(int damage_points, int CS = 0) {
        if (DodgeOpportunity) { DodgeOpportunity = false; LOG("Dodged damage ", id, damage_points); }
        else { if (CS == 0) { if (SHILED) { if (damage_points - 4 >= 0) { HEALTH -= damage_points - 4; LOG("Blocked and got damage ", id, damage_points - 4); } }
                              else { HEALTH -= damage_points; LOG("Got damage ", id, damage_points); } SHILED = false; }
        else { HEALTH -= damage_points; bleeding = true; LOG("Got Counter Strike and start bleeding ", id, damage_points); } }
    }

    void BleedStatusOff() { bleeding = false; LOG("Stop Bleeding", id); }
    bool BleedStatus() { return bleeding;}
    void Bleed() { HEALTH--; LOG("Bleeding ", id, 1);}

    bool DodgeStatus() { return DodgeOpportunity; }
    void ChangeDodgeStatus(int add) { if (add = 0) { DodgeOpportunity = false; } else { DodgeOpportunity = true; LOG("Dodge Actived", id); } }

    void ChangeConterStrikeStatus(int add) { if (add == 0) { AttackOpportunity = false; } else AttackOpportunity = true;}
    bool ConterStrikeStatus() { return AttackOpportunity; }
    void UseConterStrike(Hero& enemy) { enemy.Attacked(3, 1); }

    void AddNameSkill(string skill_name) { SkillNames.push_back(skill_name); }
    vector<string> GetSkillNames() { return SkillNames; }
};

// Magic //
class Block : public Hero
{
    int block_point;
public:
    Block() { block_point = 4;}
    void createMagic() override { cout << "Block";}
    int Magic() override { return block_point; }
};
class Healing : public Hero
{
    int heal_points;
public:
    Healing() { heal_points = 3; }
    void createMagic() override { cout << "Healing"; }
    int Magic() override { return heal_points; }
};
class Attack : public Hero
{
    int attack_poits;
public:
    Attack() { attack_poits = 5; }
    void createMagic() override { cout << "Attack"; }
    int Magic() override { return attack_poits; }
};

// Punch //
class Melee : public Hero
{
    int attack_points;
public:
    Melee() { attack_points = 4; }
    void createPunch() override { cout << "Melee"; }
    int Attack_Punch() override { return attack_points; }
};
class Range : public Hero
{
    int attack_points;
public:
    Range() { attack_points = 4; }
    void createPunch() override { cout << "Range"; }
    int Attack_Punch() override { return attack_points; }
};

// Skill //
class Dodge : public Hero
{
public:
    void createSpecialSkill() override { cout << "Dodge"; }
    bool Special() { return true; }
};
class ConterAttack : public Hero
{
public:
    void createSpecialSkill() override { cout << "ConterStrike"; }
    bool Special() { return true; }
};

class MagicFactory
{
public:
    virtual Hero* CreateMagicSkill() = 0;
    virtual ~MagicFactory() {}
};
class BlockMagicFactory : public MagicFactory
{
public:
    Hero* CreateMagicSkill() override { return new Block; }
};
class HealingMagicFactory : public MagicFactory
{
public:
    Hero* CreateMagicSkill() override { return new Healing; }
};
class AttackMagicFactory : public MagicFactory
{
public:
    Hero* CreateMagicSkill() override { return new Attack; }
};

class PunchFactory
{
public:
    virtual Hero* CreatePunchSkill() = 0;
};
class MeleePunchFactory : public PunchFactory
{
public:
    Hero* CreatePunchSkill() override { return new Melee; }
};
class RangePunchFactory : public PunchFactory
{
public:
    Hero* CreatePunchSkill() override { return new Range; }
};

class SkillFactory
{
public:
    virtual Hero* CreateSpecialSkill() = 0;
    virtual ~SkillFactory() {}
};
class DodgeSkillFactory : public SkillFactory
{
public:
    Hero* CreateSpecialSkill() override { return new Dodge; }
};
class ConterAttackSkillFactory : public SkillFactory
{
public:
    Hero* CreateSpecialSkill() override { return new ConterAttack; }
};

vector<Hero*> CreatePlayerHero(Hero &player) {
    vector<Hero*> Hero_Skills;

    char chs;
    bool ext = true;

    while (ext != false)
    {
    START:
        CreateMenu();
        cin >> chs;
        cin.ignore(32767, '\n');
        if (chs == '0')
        {
            ext = false;
        }
        else if (chs == '1')
        {
            BlockMagicFactory* magic_factory = new BlockMagicFactory;
            HealingMagicFactory* heal_factory = new HealingMagicFactory;
            AttackMagicFactory* hit_factory = new AttackMagicFactory;

            magicMenu();
            cin >> chs;
            switch (chs)
            {
            case '1': { Hero_Skills.push_back(magic_factory->CreateMagicSkill()); player.AddNameSkill("Block"); break; }
            case '2': {  Hero_Skills.push_back(heal_factory->CreateMagicSkill()); player.AddNameSkill("Heal"); break; }
            case '3': {  Hero_Skills.push_back(hit_factory->CreateMagicSkill());  player.AddNameSkill("Hit"); break; }
            default: { cerr << "Invalud Input!" << endl; }
            }


            MeleePunchFactory* short_factory = new MeleePunchFactory;
            RangePunchFactory* long_factory = new RangePunchFactory;

            punchMenu();
            cin >> chs;
            switch (chs)
            {
            case '1': {  Hero_Skills.push_back(short_factory->CreatePunchSkill()); player.AddNameSkill("Melee"); break; }
            case '2': {  Hero_Skills.push_back(long_factory->CreatePunchSkill());  player.AddNameSkill("Range"); break; }
            default: { cerr << "Invalud Input!" << endl; }
            }

            DodgeSkillFactory* dodge_factory = new DodgeSkillFactory;
            ConterAttackSkillFactory* contr_factory = new ConterAttackSkillFactory;

            skillMenu();
            cin >> chs;
            switch (chs)
            {
            case '1': {  Hero_Skills.push_back(dodge_factory->CreateSpecialSkill()); player.AddNameSkill("Dodge"); goto START; break; }
            case '2': {  Hero_Skills.push_back(contr_factory->CreateSpecialSkill()); player.AddNameSkill("ConterStrike"); goto START; break; }
            default: { cerr << "Invalud Input!" << endl; }
            }
        }
    }
    return Hero_Skills;
}
vector<Hero*> CreateCompHero(Hero &comp) {
    vector<Hero*> Hero_Skills;
    int chs;

    BlockMagicFactory* magic_factory = new BlockMagicFactory;
    HealingMagicFactory* heal_factory = new HealingMagicFactory;
    AttackMagicFactory* hit_factory = new AttackMagicFactory;

    chs = RandomNumber(3);
    switch (chs)
    {
    case 1: { Hero_Skills.push_back(magic_factory->CreateMagicSkill());  comp.AddNameSkill("Block"); break; }
    case 2: { Hero_Skills.push_back(heal_factory->CreateMagicSkill());   comp.AddNameSkill("Heal"); break; }
    case 3: { Hero_Skills.push_back(hit_factory->CreateMagicSkill());    comp.AddNameSkill("Hit"); break; }
    }

    MeleePunchFactory* short_factory = new MeleePunchFactory;
    RangePunchFactory* long_factory = new RangePunchFactory;

    chs = RandomNumber(2);
    switch (chs)
    {
    case 1: { Hero_Skills.push_back(short_factory->CreatePunchSkill()); comp.AddNameSkill("Melee"); break; }
    case 2: { Hero_Skills.push_back(long_factory->CreatePunchSkill());  comp.AddNameSkill("Range"); break; }
    }

    DodgeSkillFactory* dodge_factory = new DodgeSkillFactory;
    ConterAttackSkillFactory* contr_factory = new ConterAttackSkillFactory;

    chs = RandomNumber(2);
    switch (chs)
    {
    case 1: { Hero_Skills.push_back(dodge_factory->CreateSpecialSkill()); comp.AddNameSkill("Dodge"); break; }
    case 2: { Hero_Skills.push_back(contr_factory->CreateSpecialSkill()); comp.AddNameSkill("ConterStrike"); break; }
    }
    return Hero_Skills;
}

void GUI (Hero Player, Hero Comp, vector<int> cooldownsP, vector<int> cooldownsC){
    system("cls");
    vector<string> SSP = Player.GetSkillNames(), SSC = Comp.GetSkillNames();
    vector<Hero*> player_skills = Player.GetHero(), comp_skills = Comp.GetHero();
    cout << "\n\tYOU\t\t\t\t\tENEMY" << endl;
    cout << "\nHP: " << Player.GetHealth() << "\t\t\t\t\tHP: " << Comp.GetHealth();
    cout << "\nMagic Shield: "; Player.ShieldStatus(); cout << "\t\t\tMagic Shield: "; Comp.ShieldStatus();
    cout << "\n[1] Magic: "; player_skills[0]->createMagic(); if (cooldownsP[0] != 0) { cout << " - On Cooldown"; } 
                cout << "\t\t\tMagic: "; comp_skills[0]->createMagic(); if (cooldownsC[0] != 0) { cout << " - On Cooldown"; }
    cout << "\n[2] Punch: "; player_skills[1]->createPunch(); cout << "\t\t\tPunch: "; comp_skills[1]->createPunch();
    if (SSP[2] == "ConterStrike") { cout << "\n    Skill: "; player_skills[2]->createSpecialSkill(); if (cooldownsP[1] != 0) { cout << " - On Cooldown"; } }
    else { cout << "\n[3] Skill: "; player_skills[2]->createSpecialSkill(); if (cooldownsP[2] != 0) { cout << " - On Cooldown"; } if (Player.DodgeStatus()) { cout << " - Active"; } }
    if (SSC[2] == "ConterStrike") { cout << "\t\t\t   Skill: "; comp_skills[2]->createSpecialSkill(); if (cooldownsC[1] != 0) { cout << " - On Cooldown"; } }
    else { cout << "\t\t\t    Skill: "; comp_skills[2]->createSpecialSkill(); if (cooldownsC[2] != 0) { cout << " - On Cooldown"; } }
    cout << "\n\nStatus: "; if (cooldownsP[3] != 0) { cout << "Bleeding"; } else cout << "Helthy";
        cout << "\t\t\t\tStatus: "; if (cooldownsC[3] != 0) { cout << "Bleeding"; } else cout << "Helthy";
}

void Game(Hero &player, Hero &comp) {
    Logs* logs = logs->getInstance();
    logs->CleanLogs();
    LOG("\n\t\tTURN ", 2);
    player.SetID(0); comp.SetID(1);

    vector<Hero*>player_skills = player.GetHero(), comp_skills = comp.GetHero();

    vector<int> cooldownsP = { 0, 0, 0, 0 };  // block, CS, Dodge, Bleed
    vector<int> cooldownsC = { 0, 0, 0, 0 };

    GUI(player, comp, cooldownsP, cooldownsC);

    /*player.Attacked(comp_skills[1]->Attack_Punch());
    GUI(player, comp);*/
    vector<string> SkillNamesPlayer = player.GetSkillNames();
    vector<string> SkillNamesComp = comp.GetSkillNames();
    int turn = 0; // 0 - player     1 - comp
    int full_turn = 0;
    bool end = true;
    bool DodgeC, DodgeP;
    bool PWon = false, CWon = false;

    while (end)
    {
    GAME: 

        if (player.GetHealth() <= 0) { CWon = true; turn = 2; } if (comp.GetHealth() <= 0) { PWon = true; turn = 2; }

        DodgeP = player.DodgeStatus(); DodgeC = comp.DodgeStatus();

        if (full_turn == 2) { for (int i = 0; i < 4; i++) { if (cooldownsP[i] != 0) { cooldownsP[i] -= 1; } } full_turn = 3; }
        if (full_turn == 3) { for (int i = 0; i < 4; i++) { if (cooldownsC[i] != 0) { cooldownsC[i] -= 1; } } full_turn = 0;  LOG("\n\t\tTURN ", 2); }

        if (SkillNamesPlayer[2] == "ConterStrike") { if (cooldownsP[3] != 0) { player.Bleed(); } }
        if (SkillNamesComp[2] == "ConterStrike") { if(cooldownsC[3] != 0) { comp.Bleed(); } }


        GUI(player, comp, cooldownsP, cooldownsC);

        if (comp.ConterStrikeStatus()) { comp.UseConterStrike(player); comp.ChangeConterStrikeStatus(0); cooldownsP[3] = 4; cooldownsC[1] = 3; }
        if (player.ConterStrikeStatus()) {
            char ch; cout << "\n\nWanna use Conter Strike? [Y] [N]"; cin >> ch;
            if (tolower(ch) == 'y') { player.UseConterStrike(comp); player.ChangeConterStrikeStatus(0); cooldownsC[3] = 4; cooldownsP[1] = 3; GUI(player, comp, cooldownsP, cooldownsC); }
            else if (tolower(ch) == 'n') { player.ChangeConterStrikeStatus(0); GUI(player, comp, cooldownsP, cooldownsC); }
        }

        if (turn == 0) // player
        {
            int choose;
            cout << "\n\n\tEnter command: "; cin >> choose;
            if (choose == 1) { 
                if (SkillNamesPlayer[0] == "Block") { if (cooldownsP[0] != 0) { cout << "\nSkill on Cooldown"; goto GAME; }
                                                else { player.ArmorOn(); cooldownsP[0] = 2; turn = 1; full_turn++; } }
                else if (SkillNamesPlayer[0] == "Heal") { player.Heal(player_skills[0]->Magic()); turn = 1; full_turn++; }
                else if (SkillNamesPlayer[0] == "Hit") { comp.Attacked(player_skills[0]->Magic()); turn = 1; full_turn++; }
            }
            else if (choose == 2) {
                if (SkillNamesPlayer[1] == "Melee") {
                    comp.Attacked(player_skills[1]->Attack_Punch());
                    if (SkillNamesComp[2] == "ConterStrike" && cooldownsC[1] == 0) { comp.ChangeConterStrikeStatus(1); } turn = 1;}
                else if (SkillNamesPlayer[1] == "Range") { comp.Attacked(player_skills[1]->Attack_Punch());
                     if (SkillNamesComp[2] == "ConterStrike" && cooldownsC[1] == 0) { comp.ChangeConterStrikeStatus(1); }turn = 1; }

            }
            else if (choose == 3) {
                if (SkillNamesPlayer[2] == "Dodge") { player.ChangeDodgeStatus(1); cooldownsP[2] = 3; turn = 1; }
                else goto GAME;
            }
        } 

        else if (turn == 1) // comp
        {
            int choose = 0;
            if (SkillNamesComp[2] == "ConterStrike") { choose = RandomNumber(2); } else  choose = RandomNumber(3);
            if (choose == 1) {
                if (SkillNamesComp[0] == "Block") {
                    if (cooldownsC[0] != 0) { goto GAME; }
                    else { comp.ArmorOn(); cooldownsC[0] = 2; turn = 0; full_turn++; }
                }
                else if (SkillNamesComp[0] == "Heal") { comp.Heal(comp_skills[0]->Magic()); turn = 0; full_turn++; }
                else if (SkillNamesComp[0] == "Hit") { player.Attacked(comp_skills[0]->Magic()); turn = 0; full_turn++; }
            }
            else if (choose == 2) {
                if (SkillNamesComp[1] == "Melee") {
                    player.Attacked(comp_skills[1]->Attack_Punch());
                    if (SkillNamesPlayer[2] == "ConterStrike" && cooldownsP[1] == 0) { player.ChangeConterStrikeStatus(1); } turn = 0; }
                else if (SkillNamesComp[1] == "Range") { player.Attacked(comp_skills[1]->Attack_Punch());
                    if (SkillNamesPlayer[2] == "ConterStrike" && cooldownsP[1] == 0) { player.ChangeConterStrikeStatus(1); }turn = 0; }
            }
            else if (choose == 3) {
                if (SkillNamesComp[2] == "Dodge") { comp.ChangeDodgeStatus(1); cooldownsC[2] = 3; turn = 0; }
                else goto GAME;
            }
        }

        else if (turn == 2) {
            system("cls");
            if (CWon) { cout << "\n\t\t ENEMT WON!";LOG("\n\n\t\tCOMPUTER WINS ", 3); break; }
            if (PWon) { cout << "\n\t\t YOU WON!"; LOG("\n\n\t\tPLAYER WINS ", 3); break; }
        }

    }


}



void main() {
    
    Hero Player, Comp;
    Player.SetHeroSkills(CreatePlayerHero(Player)); Comp.SetHeroSkills(CreateCompHero(Comp));
    Game(Player, Comp);

    cin.get();
    cin.get();
}