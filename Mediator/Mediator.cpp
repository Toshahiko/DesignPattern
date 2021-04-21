#include "../smartmemory.h"

namespace {
class Mediator {
  public:
  virtual ~Mediator() = default ;
  virtual void CreateColleagues() = 0 ;
  virtual void ColleagueChanged() = 0 ;
} ;

SMART_PTR_IMPL( Mediator )

class Colleague {
  public:
  virtual ~Colleague() = default ;
  virtual void SetMediator( const Mediator_sp& mediator ) = 0 ;
  virtual void SetColleagueEnabled( bool bEnable ) = 0 ;
} ;

SMART_PTR_IMPL( Colleague )

class ColleagueButton final : public Colleague {
  public:
  void SetMediator( const Mediator_sp& mediator ) final {
    m_mediator = mediator ;
  }
  void SetColleagueEnabled( bool bEnable ) final {
    // TODO ボタンの有効無効を変更
  }

  private:
  Mediator_sp m_mediator ;
} ;
SMART_PTR_IMPL( ColleagueButton )

class ColleagueTextField final : public Colleague {
  public:
  void SetMediator( const Mediator_sp& mediator ) final {
    m_mediator = mediator ;
  }
  void SetColleagueEnabled( bool bEnable ) final {
    // TODO テキストの有効無効を変更
  }

  void TextValueChanged() {
    m_mediator->ColleagueChanged() ;
  }
  private:
  Mediator_sp m_mediator ;
} ;
SMART_PTR_IMPL( ColleagueTextField )

class ColleagueCheckbox final : public Colleague {
  public:
  void SetMediator( const Mediator_sp& mediator ) final {
    m_mediator = mediator ;
  }
  void SetColleagueEnabled( bool bEnable ) final {
    // TODO テキストの有効無効を変更
  }

  void ItemStateChanged() {
    m_mediator->ColleagueChanged() ;
  }
  private:
  Mediator_sp m_mediator ;
} ;
SMART_PTR_IMPL( ColleagueCheckbox )

class LoginFrame final : public Mediator {
  public:
  LoginFrame( const std::string& title ) {
    CreateColleagues() ;
    ColleagueChanged() ;
  }
  void CreateColleagues() final {
    m_buttonOK = std::make_shared<ColleagueButton>() ;
    m_buttonCancel = std::make_shared<ColleagueButton>() ;
    m_textUser = std::make_shared<ColleagueTextField>() ;
    m_textPass = std::make_shared<ColleagueTextField>() ;
    m_checkGuest = std::make_shared<ColleagueCheckbox>() ;
    m_checkLogin = std::make_shared<ColleagueCheckbox>() ;

    m_buttonOK.lock()->SetMediator( std::shared_ptr<Mediator>( this ) ) ;
    m_buttonCancel.lock()->SetMediator( std::shared_ptr<Mediator>( this ) ) ;
    m_textUser.lock()->SetMediator( std::shared_ptr<Mediator>( this ) ) ;
    m_textPass.lock()->SetMediator( std::shared_ptr<Mediator>( this ) ) ;
    m_checkGuest.lock()->SetMediator( std::shared_ptr<Mediator>( this ) ) ;
    m_checkLogin.lock()->SetMediator( std::shared_ptr<Mediator>( this ) ) ;
  }

  void ColleagueChanged() final {
    if ( m_checkGuest->GetState() ) {
      m_textUser.lock()->SetColleagueEnabled( false ) ;
      m_textPass.lock()->SetColleagueEnabled( false ) ;
      m_buttonCancel.lock()->SetColleagueEnabled( true ) ;
    } else {
      m_textUser.lock()->SetColleagueEnabled( true ) ;
      UserpassChanged() ;
    }
  }
  private:
  void UserpassChanged() {
    if ( m_textUser.lock().GetText().length() > 0 ) {
      m_textPass.lock()->SetColleagueEnabled( true ) ;
      if ( m_textPass.lock()->GetText().length() > 0 ) {
        m_buttonOK.lock()->SetColleagueEnabled( true ) ;
      } else {
        m_buttonOK.lock()->SetColleagueEnabled( false ) ;
      }
    } else {
      m_textPass.lock()->SetColleagueEnabled( false ) ;
      m_buttonOK.lock()->SetColleagueEnabled( false ) ;
    }
  }

  private:
  ColleagueButton_wp m_buttonOK ;
  ColleagueButton_wp m_buttonCancel ;
  ColleagueTextField_wp m_textUser ;
  ColleagueTextField_wp m_textPass ;
  ColleagueCheckbox_wp m_checkGuest ;
  ColleagueCheckbox_wp m_checkLogin ;
} ;

} // anonymous namespace

int main() {

}